"""Tests the gate, not an unavailable original solver."""
import json
import pathlib
import struct
import subprocess
import sys
import tempfile

root = pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='vsdlss-m5-test-') as temp:
    work = pathlib.Path(temp)
    job = work / 'case'
    files = {'.hdr': b'n 3\no 2\n', '.matd': struct.pack('<3d', 4, 4, 4),
             '.matf': struct.pack('<2i', 1, 2), '.matt': struct.pack('<2i', 2, 3),
             '.mato': struct.pack('<2d', -1, -1), '.rhs': struct.pack('<3d', 6, -13, 15)}
    for ext, data in files.items():
        pathlib.Path(str(job) + ext).write_bytes(data)
    manifest = work / 'manifest.json'
    manifest.write_text(json.dumps({'cases': [{'job': 'case'}]}))
    output = work / 'report.json'
    cmd = [sys.executable, str(root / 'tools/validate_m5.py'), str(manifest),
           '--solver', str(root / 'vsdlss_solver'), '--output', str(output)]
    subprocess.run(cmd, check=True)
    report = json.loads(output.read_text())
    assert report['reconstruction_regression'] == 'passed', report
    assert report['original_file_numeric'] == 'not_available'
    assert report['legacy_37_argument_abi'] == 'not_verified'
    assert len(report['cases'][0]['runs']) == 5
    p = subprocess.run(cmd + ['--require-original'])
    assert p.returncode == 3
    # Explicitly exercise refusal to accept fabricated fixture identity.
    manifest.write_text(json.dumps({'cases': [{'job': 'case', 'sha256': {'case.rhs': 'bad'}}]}))
    assert subprocess.run(cmd).returncode == 1
    # Broken output cannot be replaced by a previous successful result.
    pathlib.Path(str(job) + '.rhs').write_bytes(b'bad')
    manifest.write_text(json.dumps({'cases': [{'job': 'case'}]}))
    assert subprocess.run(cmd).returncode == 1
print('test_m5: gate/replay checks passed; original compatibility NOT verified')
