#!/usr/bin/env bash
# Memory of one process per NUMA node, from /proc/<pid>/numa_maps (no numactl
# / numastat needed).
#
#   bash numa_mem.sh [pid | process name=transient_driver] [interval s=0]
#
# interval 0: one sample.  interval > 0: a sample every interval seconds
# until the process exits (start it while the solver runs, e.g. in a
# second terminal).
set -u
WHO=${1:-transient_driver}
INT=${2:-0}
if [[ "$WHO" =~ ^[0-9]+$ ]]; then PID=$WHO; else PID=$(pgrep -n "$WHO"); fi
[ -n "${PID:-}" ] && [ -r "/proc/$PID/numa_maps" ] || { echo "no process '$WHO' (or /proc/<pid>/numa_maps not readable)"; exit 1; }
echo "pid $PID: $(tr '\0' ' ' < /proc/$PID/cmdline | cut -c1-120)"

sample() {
    awk -v t="$(date +%H:%M:%S)" '{
      ps = 4;                                                  # page size in KB (default 4 KB)
      for (i = 1; i <= NF; i++) if ($i ~ /^kernelpagesize_kB=/) { split($i, a, "="); ps = a[2] }
      for (i = 1; i <= NF; i++) if ($i ~ /^N[0-9]+=/) { split($i, a, "="); kb[a[1]] += a[2] * ps }
    } END {
      for (k in kb) tot += kb[k];
      if (tot == 0) { print t "  (no pages yet)"; exit }
      line = t;
      for (k in kb) line = line sprintf("   %s %7.2f GB %5.1f%%", k, kb[k] / 1048576, 100 * kb[k] / tot);
      printf "%s   total %.2f GB\n", line, tot / 1048576
    }' "/proc/$PID/numa_maps" 2>/dev/null
}

if [ "$INT" = 0 ]; then sample; exit 0; fi
while [ -r "/proc/$PID/numa_maps" ]; do sample || break; sleep "$INT"; done
echo "process $PID exited"
