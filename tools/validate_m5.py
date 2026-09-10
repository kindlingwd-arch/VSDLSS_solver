#!/usr/bin/env python3
"""Reproducible file/numeric differential gate; never asserts legacy ABI compatibility.

An optional original executable must have an explicit argv adapter and SHA-256.
Commands run without a shell in separate temporary job directories. Original
outputs must be adapted to little-endian binary64 <job>.rsl by that command.
"""
import argparse
import hashlib
import json
import math
import pathlib
import shutil
import struct
import subprocess
import tempfile

SUFFIXES = ('.hdr', '.matd', '.matf', '.matt', '.mato', '.rhs')


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def unpack(path, code, count):
    raw = path.read_bytes()
    if len(raw) != struct.calcsize('<' + code) * count:
        raise ValueError(f'wrong byte length: {path.name}')
    values = struct.unpack(f'<{count}{code}', raw)
    if code == 'd' and not all(map(math.isfinite, values)):
        raise ValueError(f'non-finite value: {path.name}')
    return values


def matrix(job):
    fields = dict(line.split() for line in pathlib.Path(str(job) + '.hdr').read_text().splitlines() if line.strip())
    n, off = int(fields['n']), int(fields['o'])
    if n < 1 or off < 0:
        raise ValueError('invalid dimensions')
    read = lambda ext, code, size: unpack(pathlib.Path(str(job) + ext), code, size)
    diag, rhs = read('.matd', 'd', n), read('.rhs', 'd', n)
    rows, cols, vals = read('.matf', 'i', off), read('.matt', 'i', off), read('.mato', 'd', off)
    if any(r < 1 or r > n or c < 1 or c > n or r == c for r, c in zip(rows, cols)):
        raise ValueError('invalid off-diagonal index')
    return n, diag, rhs, rows, cols, vals


def backward_error(data, x):
    n, diag, rhs, rows, cols, vals = data
    ax = [diag[i] * x[i] for i in range(n)]
    norm_rows = list(map(abs, diag))
    for r, c, v in zip(rows, cols, vals):
        r -= 1
        c -= 1
        ax[r] += v * x[c]
        ax[c] += v * x[r]
        norm_rows[r] += abs(v)
        norm_rows[c] += abs(v)
    residual = max(abs(b - a) for b, a in zip(rhs, ax))
    denominator = max(norm_rows) * max(map(abs, x)) + max(map(abs, rhs))
    if not math.isfinite(residual) or not math.isfinite(denominator):
        raise ValueError('non-finite residual computation')
    return residual / denominator if denominator else (0.0 if not residual else math.inf)


def run_case(case, root, solver, budget, timeout, oracle):
    source = (root / case['job']).resolve()
    data = matrix(source)
    files = [pathlib.Path(str(source) + ext) for ext in SUFFIXES]
    # Additional original-specific inputs must be explicitly listed and hashed.
    for extra in case.get('extra_files', []):
        files.append((root / extra).resolve())
    names = [p.name for p in files]
    if len(set(names)) != len(names):
        raise ValueError('duplicate fixture basenames')
    hashes = {p.name: digest(p) for p in files}
    declared = case.get('sha256', {})
    if any(hashes.get(name) != value for name, value in declared.items()):
        raise ValueError('fixture hash mismatch')
    if oracle and declared != hashes:
        raise ValueError('original comparison requires hashes for every fixture')
    result = {'job': case['job'], 'fixture_sha256': hashes, 'runs': []}
    commands = [('m1', []), ('m3', ['--m3']),
                ('m4_width1', ['--disk-budget', str(budget), '--block-cols', '1']),
                ('m4_width8', ['--disk-budget', str(budget), '--block-cols', '8', '--save-factor', 'factor.v2'])]
    reference = None
    for name, options in commands + ([('original', [])] if oracle else []):
        with tempfile.TemporaryDirectory(prefix='vsdlss-m5-') as temp:
            work = pathlib.Path(temp)
            for src in files:
                shutil.copyfile(src, work / src.name)
            job = work / source.name
            pathlib.Path(str(job) + ".rsl").unlink(missing_ok=True)
            if name == 'original':
                argv = [arg.replace('{job}', str(job)) for arg in oracle['argv']]
            else:
                argv = [str(solver), *options, str(job)]
            completed = subprocess.run(argv, cwd=work, capture_output=True, text=True, timeout=timeout)
            if completed.returncode:
                raise ValueError(f'{name} exit {completed.returncode}: {completed.stderr[-2000:]}')
            x = unpack(pathlib.Path(str(job) + '.rsl'), 'd', data[0])
            eta = backward_error(data, x)
            if reference is None:
                reference = x
            error = max(abs(a - b) for a, b in zip(reference, x)) / max(1.0, max(map(abs, reference)))
            if eta > 1e-12 or error > 1e-10:
                raise ValueError(f'{name}: backward_error={eta}, relative_difference={error}')
            result['runs'].append({'mode': name, 'backward_error': eta, 'relative_difference': error,
                                   'stdout': completed.stdout[-2000:]})
            if name == 'm4_width8':
                # New process, fresh solution; stale output cannot mask failure.
                pathlib.Path(str(job) + '.rsl').unlink()
                replay = subprocess.run([str(solver), '--disk-budget', str(budget), '--load-factor',
                                         'factor.v2', str(job)], cwd=work, capture_output=True, text=True, timeout=timeout)
                if replay.returncode:
                    raise ValueError(f'factor replay exit {replay.returncode}')
                y = unpack(pathlib.Path(str(job) + '.rsl'), 'd', data[0])
                if y != x:
                    raise ValueError('factor replay differs from saved solve')
                result['runs'].append({'mode': 'm4_reopen', 'exact_replay': True})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('manifest', type=pathlib.Path)
    parser.add_argument('--solver', type=pathlib.Path, default=pathlib.Path('./vsdlss_solver'))
    parser.add_argument('--output', type=pathlib.Path, required=True)
    parser.add_argument('--budget', type=int, default=16 * 1024 * 1024)
    parser.add_argument('--timeout', type=float, default=60)
    parser.add_argument('--require-original', action='store_true')
    args = parser.parse_args()
    report = {'legacy_37_argument_abi': 'not_verified', 'original_file_numeric': 'not_available', 'cases': []}
    code = 1
    try:
        if args.budget < 1 or not math.isfinite(args.timeout) or args.timeout <= 0:
            raise ValueError('budget and timeout must be positive')
        manifest = json.loads(args.manifest.read_text())
        root, solver = args.manifest.resolve().parent, args.solver.resolve()
        report['solver_sha256'] = digest(solver)
        oracle = manifest.get('oracle')
        if oracle:
            if (oracle.get('kind') != 'original' or not isinstance(oracle.get('argv'), list) or
                    not oracle['argv'] or not all(isinstance(a, str) and a for a in oracle['argv']) or
                    not any('{job}' in a for a in oracle['argv'])):
                raise ValueError('oracle requires kind=original and explicit argv')
            binary = (root / oracle['argv'][0]).resolve()
            if 'rebuilt' in binary.name or digest(binary) != oracle.get('sha256'):
                raise ValueError('original executable hash mismatch or rebuilt artifact')
            oracle['argv'][0] = str(binary)
            report['oracle_sha256'] = digest(binary)
        cases = manifest.get('cases', [])
        if not cases:
            raise ValueError('manifest contains no cases')
        for case in cases:
            report['cases'].append(run_case(case, root, solver, args.budget, args.timeout, oracle))
        report['reconstruction_regression'] = 'passed'
        if oracle:
            report['original_file_numeric'] = 'passed_for_supplied_adapter_and_fixtures'
        code = 3 if args.require_original and not oracle else 0
        if code == 3:
            report['error'] = 'original executable and hashed real fixtures are required; M5 is not accepted'
    except (OSError, ValueError, KeyError, TypeError, subprocess.TimeoutExpired) as error:
        report['error'] = str(error)
        report['reconstruction_regression'] = 'failed'
    args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2, allow_nan=False) + '\n')
    return code


if __name__ == '__main__':
    raise SystemExit(main())
