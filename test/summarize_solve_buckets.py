#!/usr/bin/env python3
"""Summarize raw profile logs; aggregate each run before taking medians."""
import csv
import sys
from pathlib import Path
from statistics import median

writer = csv.writer(sys.stdout, lineterminator='\n')
writer.writerow(['nodes', 'baseline_ms', 'wide_back_diag_ms', 'wide_share_pct',
                 'estimated_4x_saved_ms', 'estimated_4x_saved_pct', 'profile_drift_pct'])
for filename in sys.argv[1:]:
    lines = Path(filename).read_text().splitlines()
    def rows(tag):
        return [dict(item.split('=', 1) for item in line.split()[1:])
                for line in lines if line.startswith(tag + ' ')]
    baseline = float(rows('PROFILE_BASELINE')[0]['median'])
    runs = rows('PROFILE_RUN')
    buckets = rows('PROFILE_BUCKET')
    assert len(runs) == 7 and all(r['checked'] == 'bitwise' for r in runs)
    assert len(buckets) == 49
    # Width >=256 is buckets 5 and 6; diagonal backward solve only.
    diagonal = median(sum(float(b['back_diag']) for b in buckets
                          if b['run'] == r['run'] and int(b['bucket']) >= 5)
                      for r in runs)
    drift = median(float(r['total']) for r in runs) / baseline - 1
    values = [baseline * 1000, diagonal * 1000, diagonal / baseline * 100,
              diagonal * 750, diagonal / baseline * 75, drift * 100]
    writer.writerow([rows('PROFILE_STRUCTURE')[0]['n']] +
                    [f'{v:.6f}' for v in values])
