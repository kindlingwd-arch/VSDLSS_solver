#!/usr/bin/env python3
"""End-to-end text dump replay through the production test binary."""
import math
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
FIXTURES = ROOT / 'test' / 'fixtures' / 'text_dumps'


class TextDumpTests(unittest.TestCase):
    def invoke(self, directory, *options):
        with tempfile.TemporaryDirectory() as tmp:
            return subprocess.run(
                [str(ROOT / 'vsdlss_solver'), '--text-dump', str(directory), *options,
                 str(Path(tmp) / 'solution')],
                text=True, capture_output=True, cwd=ROOT, timeout=30,
            )

    def test_valid_cases_all_factor_paths(self):
        for name in ['01_two_nodes', '02_tree_five_nodes', '03_grid_eight_nodes']:
            for mode in [(), ('--m3',), ('--disk-budget', '8192'),
                         ('--disk-budget', '8192', '--m4-reduced')]:
                with self.subTest(case=name, mode=mode):
                    p = self.invoke(FIXTURES / name, *mode)
                    self.assertEqual(p.returncode, 0, p.stderr)
                    metrics = dict(re.findall(r'\b([a-z_]+_ms|relative_residual_inf|max_abs_error_vs_x|relative_error_vs_x_inf)=([0-9.eE+-]+)', p.stdout))
                    self.assertEqual(set(metrics), {'load_ms', 'prepare_ms', 'factor_ms', 'solve_ms', 'factor_solve_ms', 'relative_residual_inf', 'max_abs_error_vs_x', 'relative_error_vs_x_inf'})
                    for key in metrics:
                        self.assertTrue(math.isfinite(float(metrics[key])), (key, p.stdout))
                    self.assertLess(float(metrics['relative_residual_inf']), 1e-12)
                    self.assertLess(float(metrics['max_abs_error_vs_x']), 1e-12)
                    self.assertGreaterEqual(float(metrics['factor_solve_ms']), float(metrics['factor_ms']))
                    self.assertGreaterEqual(float(metrics['factor_solve_ms']), float(metrics['solve_ms']))

    def test_rejects_invalid_dump(self):
        with tempfile.TemporaryDirectory() as tmp:
            d = Path(tmp)
            for filename in ['diag.txt', 'data.txt', 'b_vector.txt', 'x_vector.txt']:
                (d / filename).write_bytes((FIXTURES / '01_two_nodes' / filename).read_bytes())
            (d / 'data.txt').write_text('0 1 1\n0 1 1\n')
            p = self.invoke(d)
            self.assertNotEqual(p.returncode, 0)
            self.assertIn('data.txt', p.stderr)
            (d / 'data.txt').write_text('0 1 1\n')
            (d / 'b_vector.txt').write_text('0 2\n0 5\n')
            p = self.invoke(d)
            self.assertNotEqual(p.returncode, 0)
            self.assertIn('b_vector.txt', p.stderr)
            (d / 'b_vector.txt').write_text('0 2\n1 5\n')
            (d / 'x_vector.txt').write_text('0 1\n1 nan\n')
            p = self.invoke(d)
            self.assertNotEqual(p.returncode, 0)
            self.assertIn('x_vector.txt', p.stderr)

    def test_plain_diagonal_and_reordered_edges(self):
        with tempfile.TemporaryDirectory() as tmp:
            d = Path(tmp)
            source = FIXTURES / '02_tree_five_nodes'
            for filename in ['data.txt', 'b_vector.txt', 'x_vector.txt']:
                (d / filename).write_bytes((source / filename).read_bytes())
            (d / 'diag.txt').write_text('6\n4\n8\n2\n5\n')
            # The parser accepts arbitrary row ordering in data.txt.
            (d / 'data.txt').write_text(''.join(reversed((d / 'data.txt').read_text().splitlines(keepends=True))))
            p = self.invoke(d)
            self.assertEqual(p.returncode, 0, p.stderr)
            self.assertIn('relative_residual_inf=', p.stdout)


if __name__ == '__main__':
    unittest.main()
