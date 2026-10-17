#!/usr/bin/env python3
"""Static analysis of a pipelined M3 solve (no solver code needed).

Reads the structure written by export_pipeline (core supernode tree and the
replay units) and answers, per component:

  1. forward readiness: when x% of the forward replay is done, how much of the
     core forward work could already start (all its subtree's inputs in);
  2. backward readiness: when x% of the core backward work is done, how much of
     the backward replay could already start (the core values it reads final);
  3. a schedule simulation on P threads (the core on at most --core-max of
     them): the current order with a wait between the four phases
     (forward replay, core forward, core backward, backward replay) against
     the same tasks run as soon as their inputs are ready.

Task costs: replay units by records, core supernodes by stored entries,
calibrated so that the phase-by-phase simulation matches the measured times
(--fwd, --core, --bwd: milliseconds of one component's phases at P threads,
e.g. from VSDLSS_TRACE=1; the core at --core-max threads with --core-cap).
A model, not a measurement: tasks run on one thread each, scaling is linear
except for the core's thread cap, memory bandwidth is not modelled.

  python3 pipeline_analysis.py export.txt --threads 16 --fwd 40 --core 28 --bwd 32 [--core-cap 23]
"""
import argparse
import heapq
import sys


def read_export(path):
    comps = []
    with open(path) as f:
        line = f.readline()
        while line:
            if not line.startswith("component"):
                line = f.readline()
                continue
            t = line.split()
            hdr = {t[i]: int(t[i + 1]) for i in range(0, len(t) - 1, 2)}
            S, U = hdr["supernodes"], hdr["units"]
            parent, work = [0] * S, [0] * S
            for s in range(S):
                p, w = f.readline().split()
                parent[s], work[s] = int(p), int(w)
            units = []
            for _ in range(U):
                u = f.readline().split()
                m = int(u[3])
                units.append((u[0], int(u[1]), int(u[2]), [int(x) for x in u[4:4 + m]]))
            comps.append((hdr, parent, work, units))
            line = f.readline()
    return comps


def postorder(parent):
    S = len(parent)
    kids = [[] for _ in range(S)]
    roots = []
    for s, p in enumerate(parent):
        (kids[p] if 0 <= p < S else roots).append(s)
    order, stack = [], [(r, 0) for r in reversed(roots)]
    while stack:
        s, i = stack.pop()
        if i < len(kids[s]):
            stack.append((s, i + 1))
            stack.append((kids[s][i], 0))
        else:
            order.append(s)
    return order, kids, roots


def simulate(dur, is_core, succ, indeg0, prio, P, core_max):
    """List scheduling: a task is ready when its predecessors are done; free
    threads take ready core tasks first (at most core_max at once), then the
    others, each kind by ascending prio.  Returns finish times and the
    makespan."""
    n = len(dur)
    indeg = list(indeg0)
    ready_c, ready_o = [], []          # (prio, task)
    fin = [0.0] * n
    for v in range(n):
        if indeg[v] == 0:
            heapq.heappush(ready_c if is_core[v] else ready_o, (prio[v], v))
    running = []                       # (finish, task)
    t, free, core_run, done = 0.0, P, 0, 0
    while done < n:
        while free > 0:
            if ready_c and core_run < core_max:
                _, v = heapq.heappop(ready_c); core_run += 1
            elif ready_o:
                _, v = heapq.heappop(ready_o)
            else:
                break
            free -= 1
            heapq.heappush(running, (t + dur[v], v))
        if not running:
            raise RuntimeError("deadlock in the task graph")
        t = running[0][0]
        while running and running[0][0] <= t:
            f, v = heapq.heappop(running)
            fin[v] = f; done += 1; free += 1
            if is_core[v]: core_run -= 1
            for w in succ[v]:
                indeg[w] -= 1
                if indeg[w] == 0:
                    heapq.heappush(ready_c if is_core[w] else ready_o, (prio[w], w))
    return fin, max(fin) if n else 0.0


def phase(dur_list, succ_edges, is_core_flag, P, core_max):
    n = len(dur_list)
    succ = [[] for _ in range(n)]
    indeg = [0] * n
    for a, b in succ_edges:
        succ[a].append(b); indeg[b] += 1
    return simulate(dur_list, [is_core_flag] * n, succ, indeg, list(range(n)), P, core_max)


def curve_points(times_done, weights_done, times_ready, weights_ready, marks):
    """For each mark x: fraction of the ready-side weight whose ready time is
    <= the time at which x of the done-side weight has finished."""
    order = sorted(range(len(times_done)), key=lambda i: times_done[i])
    total = sum(weights_done) or 1
    acc, k, at = 0.0, 0, []
    for x in marks:
        while k < len(order) and acc / total < x:
            acc += weights_done[order[k]]; k += 1
        at.append(times_done[order[k - 1]] if k else 0.0)
    tot_r = sum(weights_ready) or 1
    out = []
    for tm in at:
        out.append(sum(w for t, w in zip(times_ready, weights_ready) if t <= tm + 1e-12) / tot_r)
    return out


def analyse(hdr, parent, work, units, a):
    S = len(parent)
    post, kids, roots = postorder(parent)
    po = [0] * S
    for i, s in enumerate(post):
        po[s] = i
    P, CM = a.threads, min(a.core_max, a.threads)
    blocks = [i for i, u in enumerate(units) if u[0] == "B"]
    tails = [i for i, u in enumerate(units) if u[0] == "T"]
    # tail units that feed early (deep) parts of the core tree first; units
    # touching no core vertex last (they gate nothing in the core)
    tails.sort(key=lambda i: min((po[s] for s in units[i][3]), default=S))
    order = blocks + tails
    rec = [u[1] for u in units]
    total_rec = float(sum(rec)) or 1.0

    # --- calibration ------------------------------------------------------
    # replays: linear scaling, so per-record cost = time * P / records
    af = a.fwd * P / total_rec
    ab = a.bwd * P / total_rec
    # core: simulate forward and backward alone on CM threads with unit cost
    cf_edges = [(s, parent[s]) for s in range(S) if 0 <= parent[s] < S]
    cb_edges = [(parent[s], s) for s in range(S) if 0 <= parent[s] < S]
    _, tf1 = phase([float(w) for w in work], cf_edges, True, CM, CM)
    _, tb1 = phase([float(w) for w in work], cb_edges, True, CM, CM)
    core_t = a.core_cap if a.core_cap else a.core
    kf = (core_t / 2) / tf1 if tf1 else 0.0
    kb = (core_t / 2) / tb1 if tb1 else 0.0

    # --- 1. forward readiness ---------------------------------------------
    # replay units in `order` on P threads (blocks first, as now)
    U = len(units)
    fdur = [rec[i] * af for i in range(U)]
    edges = []
    if blocks and tails:                   # tail units wait for the blocks
        bar = U                            # zero-cost barrier task
        fdur.append(0.0)
        edges += [(b, bar) for b in blocks] + [(bar, t) for t in tails]
    n = len(fdur)
    pr = [0] * n                           # replay in the chosen order
    for k, i in enumerate(order):
        pr[i] = k
    succ = [[] for _ in range(n)]; indeg = [0] * n
    for x, y in edges:
        succ[x].append(y); indeg[y] += 1
    ffin, t_frep = simulate(fdur, [False] * n, succ, indeg, pr, P, CM)
    in_ready = [0.0] * S
    for i in range(U):
        for s in units[i][3]:
            if ffin[i] > in_ready[s]:
                in_ready[s] = ffin[i]
    sub_ready = in_ready[:]
    for s in post:
        p = parent[s]
        if 0 <= p < S and sub_ready[s] > sub_ready[p]:
            sub_ready[p] = sub_ready[s]
    marks = [0.25, 0.5, 0.75, 0.9, 1.0]
    fcurve = curve_points(ffin[:U], rec, sub_ready, work, marks)

    # --- 2. backward readiness --------------------------------------------
    bfin, t_cb = phase([w * kb for w in work], cb_edges, True, CM, CM)
    ready_b = [max((bfin[s] for s in units[i][3]), default=0.0) for i in range(U)]
    bcurve = curve_points(bfin, work, ready_b, rec, marks)

    # --- 3. whole solve: phases with waits vs dataflow ----------------------
    _, t_cf = phase([w * kf for w in work], cf_edges, True, CM, CM)
    _, t_brep = phase([rec[i] * ab for i in range(U)], [], False, P, CM)
    barrier = t_frep + t_cf + t_cb + t_brep
    # dataflow graph: F units | CF supernodes | CB supernodes | B units (+ barriers)
    F0, CF0, CB0, B0 = 0, U, U + S, U + 2 * S
    nt = U + 2 * S + U + 2
    BAR_F, BAR_B = nt - 2, nt - 1          # blocks -> tail (forward), tail -> blocks (backward)
    dur = [0.0] * nt; core = [False] * nt
    for i in range(U):
        dur[F0 + i] = rec[i] * af; dur[B0 + i] = rec[i] * ab
    for s in range(S):
        dur[CF0 + s] = work[s] * kf; dur[CB0 + s] = work[s] * kb
        core[CF0 + s] = core[CB0 + s] = True
    E = []
    for b in blocks:
        E.append((F0 + b, BAR_F)); E.append((BAR_B, B0 + b))
    for t_ in tails:
        E.append((BAR_F, F0 + t_)); E.append((B0 + t_, BAR_B))
    for i in range(U):
        for s in units[i][3]:
            E.append((F0 + i, CF0 + s))      # its updates reach core column s
            E.append((CB0 + s, B0 + i))      # it reads core values of s
    for s in range(S):
        p = parent[s]
        if 0 <= p < S:
            E.append((CF0 + s, CF0 + p)); E.append((CB0 + p, CB0 + s))
        else:
            E.append((CF0 + s, CB0 + s))     # root: backward after its forward
    for i in range(U):                       # a unit's backward after its own forward
        E.append((F0 + i, B0 + i))
    succ = [[] for _ in range(nt)]; indeg = [0] * nt
    for x, y in E:
        succ[x].append(y); indeg[y] += 1
    pr = list(range(nt))                     # forward units in the chosen order, before the backward units
    for k, i in enumerate(order):
        pr[F0 + i] = k
    for s_ in range(S):                      # core: forward bottom-up, backward top-down
        pr[CF0 + s_] = po[s_]; pr[CB0 + s_] = S - po[s_]
    _, pipe = simulate(dur, core, succ, indeg, pr, P, CM)

    print("component %d: n %d, records %d, core %d in %d supernodes, %d block segments, %d tail units" %
          (hdr["component"], hdr["n"], hdr["records"], hdr["core"], S, len(blocks), len(tails)))
    print("  model phases (ms, %d threads, core on %d): fwd replay %.1f, core fwd %.1f, core bwd %.1f, bwd replay %.1f"
          % (P, CM, t_frep, t_cf, t_cb, t_brep))
    print("  1. core forward work ready when the forward replay is   " +
          "  ".join("%d%%: %4.1f%%" % (100 * m, 100 * v) for m, v in zip(marks, fcurve)))
    print("  2. backward replay ready when the core backward is      " +
          "  ".join("%d%%: %4.1f%%" % (100 * m, 100 * v) for m, v in zip(marks, bcurve)))
    print("  3. whole solve: phases with waits %.1f ms | dataflow %.1f ms | saving %.1f ms (%.1f%%)"
          % (barrier, pipe, barrier - pipe, 100 * (barrier - pipe) / barrier if barrier else 0))
    return barrier, pipe


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("export")
    ap.add_argument("--threads", type=int, required=True, help="threads of one component (e.g. 16 of 32 for two nets)")
    ap.add_argument("--fwd", type=float, required=True, help="measured forward replay, ms")
    ap.add_argument("--core", type=float, required=True, help="measured core solve (fwd+bwd), ms, at --threads")
    ap.add_argument("--bwd", type=float, required=True, help="measured backward replay, ms")
    ap.add_argument("--core-max", type=int, default=8, help="threads the core can use (default 8)")
    ap.add_argument("--core-cap", type=float, default=0, help="measured core solve at --core-max threads, ms (default: --core)")
    ap.add_argument("--component", type=int, default=-1, help="only this component")
    a = ap.parse_args()
    comps = read_export(a.export)
    if not comps:
        sys.exit("no components in " + a.export)
    for hdr, parent, work, units in comps:
        if a.component >= 0 and hdr["component"] != a.component:
            continue
        analyse(hdr, parent, work, units, a)


if __name__ == "__main__":
    main()
