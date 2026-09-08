# M2 Ordering Reconstruction Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add verified dynamic minimum-degree and multilevel nested-dissection orderings to the M1 SPD solver, with structural statistics tied to the actual Cholesky factor.

**Architecture:** A private mutable simple-graph module owns adjacency and fill insertion. The minimum-degree module performs exact elimination-graph updates; the MLD module recursively partitions induced subgraphs and uses minimum degree at leaves and separators. The existing ordering façade validates inputs, dispatches selectors, computes statistics, and supplies the unchanged factorization pipeline.

**Tech Stack:** C11, existing upper-CSC matrix and sparse Cholesky core, GNU Make, native assertion-based C tests, ASan/UBSan.

**Spec:** `docs/superpowers/specs/2026-09-08-m2-ordering-design.md`

## Global Constraints

- Preserve `q[new] = old` and `pinv[old] = new` for every selector.
- Do not modify caller-owned matrices or vectors.
- Preserve selectors `1 = RCM` and `2 = natural`; add `3 = dynamic minimum degree`, `4 = MLD`; switch `0` to MLD only in the final integration task.
- Implement only independently owned code derived from evidenced principles; do not compile or copy `reference/` sources.
- Keep all public operations transactional: outputs remain null or unchanged on failure.
- Use checked `csi` to `size_t` conversions and return `VSDLSS_ERR_OOM` or `VSDLSS_ERR_INVALID` rather than overflowing.
- Require strict C11 `-Wall -Wextra -Werror`, the full regression suite, ASan, and UBSan before completion.

---

### Task 1: Mutable ordering graph

**Files:**
- Create: `src/vsdlss_graph.c`
- Modify: `src/vsdlss_internal.h`
- Modify: `Makefile`
- Test: `test/test_ordering.c`

**Interfaces:**
- Consumes: validated upper CSC `const vsdlss *A`.
- Produces: opaque `vsdlss_graph`; `vsdlss_graph_build`, `vsdlss_graph_free`, `vsdlss_graph_degree`, `vsdlss_graph_active_neighbors`, `vsdlss_graph_eliminate`, and `vsdlss_graph_fill_edges` private functions declared in `src/vsdlss_internal.h`.

- [ ] **Step 1: Write graph construction and elimination tests**

Add fixtures that build a path and a four-cycle, assert off-diagonal symmetry and exact degrees, eliminate one vertex, and assert its active-neighbor clique and fill count. The production change that makes these tests pass is a graph builder that deduplicates structural edges and an elimination operation that inserts missing clique edges.

```c
CHECK(vsdlss_graph_degree(graph, 0) == 2);
CHECK(vsdlss_graph_eliminate(graph, 0, &neighbors, &count) == VSDLSS_OK);
CHECK(count == 2);
CHECK(vsdlss_graph_has_edge(graph, neighbors[0], neighbors[1]));
CHECK(vsdlss_graph_fill_edges(graph) == 1);
```

- [ ] **Step 2: Run the ordering test and verify RED**

Run: `make test_ordering`

Expected: compilation fails because `vsdlss_graph` and its functions do not exist.

- [ ] **Step 3: Implement the graph module**

Use sorted growable `csi` neighbor vectors, an active byte vector, checked allocation helpers, binary-search membership, and unique insertion. `vsdlss_graph_eliminate` first snapshots active neighbors, inserts every missing neighbor pair, then deactivates the vertex; it never removes historical adjacency.

- [ ] **Step 4: Verify GREEN and the full M1 suite**

Run: `make test_ordering && make test-unit test-io`

Expected: all three test binaries report `ALL OK`.

- [ ] **Step 5: Commit**

```bash
git add Makefile src/vsdlss_graph.c src/vsdlss_internal.h test/test_ordering.c
git commit -m "feat: add mutable elimination graph"
```

### Task 2: Exact dynamic minimum-degree ordering

**Files:**
- Create: `src/vsdlss_min_degree.c`
- Modify: `src/vsdlss_internal.h`
- Modify: `src/vsdlss_ordering.c`
- Modify: `include/vsdlss.h`
- Modify: `Makefile`
- Test: `test/test_ordering.c`

**Interfaces:**
- Consumes: `vsdlss_graph` and normalized upper CSC.
- Produces: private `vsdlss_min_degree_order(const vsdlss *A, const csi *vertices, csi count, csi **q, vsdlss_order_stats *stats)` and public selector `3` through `vsdlss_order_analyze`.

- [ ] **Step 1: Add an independent boolean-elimination oracle test**

The test owns a dense boolean adjacency oracle for graphs of at most 12 vertices. At each step it selects the active minimum degree with original-index tie breaking, completes the neighbor clique, and records order/fill. Compare selector `3` against it on a fill-producing six-vertex fixture.

```c
CHECK(vsdlss_order_analyze(A, 3, &q, &pinv, &stats) == VSDLSS_OK);
oracle_min_degree(A, expected_q, &expected_fill);
CHECK(equal_csi(q, expected_q, A->n));
CHECK(stats.fill_edges_added == expected_fill);
```

- [ ] **Step 2: Verify RED**

Run: `make test_ordering`

Expected: compilation fails because `vsdlss_order_stats` and `vsdlss_order_analyze` are absent.

- [ ] **Step 3: Add the public stats contract and minimum-degree kernel**

Add the exact struct and function signatures from the spec. Scan active vertices for the actual current minimum degree, break ties by original index, add that degree to `predicted_nnz_l`, and call graph elimination. On failure, free local storage and leave `q`/`pinv` null.

- [ ] **Step 4: Integrate selector 3 without changing selector 0**

`vsdlss_order` delegates to `vsdlss_order_analyze`. Dispatch `0/1` to RCM, `2` to identity, and `3` to minimum degree. Build `pinv`, validate the bijection, and publish stats only after success.

- [ ] **Step 5: Verify GREEN and factor compatibility**

Run: `make test_ordering && make test`

Expected: oracle comparison passes; all prior solver and file tests remain green.

- [ ] **Step 6: Commit**

```bash
git add Makefile include/vsdlss.h src/vsdlss_internal.h src/vsdlss_min_degree.c src/vsdlss_ordering.c test/test_ordering.c
git commit -m "feat: add dynamic minimum-degree ordering"
```

### Task 3: Structural statistics cross-check

**Files:**
- Modify: `src/vsdlss_ordering.c`
- Modify: `src/vsdlss_factor.c`
- Test: `test/test_ordering.c`
- Test: `test/test_solver.c`

**Interfaces:**
- Consumes: successful ordering, `vsdlss_etree`, and the existing factor accessors.
- Produces: exact `predicted_nnz_l`, `fill_edges_added`, and `elimination_tree_height`; selector `3` support in `vsdlss_factorize`.

- [ ] **Step 1: Add predicted-versus-actual factor tests**

For path, arrowhead, and 5x5 grid SPD matrices, order with selector `3`, factor with selector `3`, and compare `stats.predicted_nnz_l` to `vsdlss_factor_L(factor)->p[n]`. Check tree height is in `[1,n]`.

- [ ] **Step 2: Verify RED**

Run: `make test_ordering test-unit`

Expected: selector `3` factorization returns `VSDLSS_ERR_UNSUPPORTED` or statistics mismatch.

- [ ] **Step 3: Compute tree height and enable factor selector 3**

Apply `pinv` with `vsdlss_symperm`, compute the elimination tree with `vsdlss_etree`, then compute each node depth with parent-chain memoization and overflow guards. Extend factor selector validation to accept `3`.

- [ ] **Step 4: Verify GREEN**

Run: `make test_ordering test-unit`

Expected: predicted and actual `nnz(L)` agree exactly and all numerical checks pass.

- [ ] **Step 5: Commit**

```bash
git add src/vsdlss_ordering.c src/vsdlss_factor.c test/test_ordering.c test/test_solver.c
git commit -m "test: cross-check ordering structure with factors"
```

### Task 4: Recursive MLD ordering

**Files:**
- Create: `src/vsdlss_mld.c`
- Modify: `src/vsdlss_internal.h`
- Modify: `src/vsdlss_ordering.c`
- Modify: `src/vsdlss_factor.c`
- Modify: `Makefile`
- Test: `test/test_ordering.c`

**Interfaces:**
- Consumes: immutable simple graph structure, induced vertex lists, and the minimum-degree leaf kernel.
- Produces: private `vsdlss_mld_order(const vsdlss *A, csi **q, vsdlss_order_stats *stats)` and selector `4`.

- [ ] **Step 1: Add MLD permutation and separator tests**

Test a 9x9 grid, disconnected paths plus isolates, and an adversarial lopsided tree. Assert a valid permutation, nonzero separator count for the grid, and exact solution equivalence through selector `4`. A test-only separator checker removes the recorded top separator fixture and proves no left-right edge remains.

- [ ] **Step 2: Verify RED**

Run: `make test_ordering`

Expected: selector `4` returns `VSDLSS_ERR_UNSUPPORTED`.

- [ ] **Step 3: Implement connected-component and BFS helpers**

Use deterministic queues sized to the current induced set, map membership with generation marks, split disconnected components by smallest original vertex, and find pseudo-peripheral endpoints through repeated farthest-level BFS.

- [ ] **Step 4: Implement recursive partition and fallback**

For more than 32 vertices, choose the middle BFS level as separator. If balance violates the 7/8 rule, split BFS order near half and move every vertex incident across the provisional cut into the separator until no cross edge remains. Recursively emit left, right, then a minimum-degree ordering of the separator.

- [ ] **Step 5: Integrate selector 4 and verify GREEN**

Run: `make test_ordering && make test`

Expected: all MLD structural and numerical tests pass; selectors 1/2/3 remain unchanged.

- [ ] **Step 6: Commit**

```bash
git add Makefile src/vsdlss_mld.c src/vsdlss_internal.h src/vsdlss_ordering.c src/vsdlss_factor.c test/test_ordering.c
git commit -m "feat: add multilevel nested-dissection ordering"
```

### Task 5: Default integration, benchmark, and documentation

**Files:**
- Modify: `src/vsdlss_ordering.c`
- Modify: `src/main.c`
- Modify: `README.md`
- Modify: `docs/reconstruction/01-principles-and-evidence.md`
- Modify: `docs/reconstruction/03-design-and-roadmap.md`
- Modify: `docs/reconstruction/CONTINUATION.md`
- Create: `docs/reconstruction/04-ordering-evidence-map.md`
- Test: `test/test_ordering.c`
- Test: `test/test_solver.c`

**Interfaces:**
- Consumes: completed selectors 1/2/3/4 and ordering statistics.
- Produces: selector `0 = MLD`, demo reporting, and an evidence-to-implementation-to-test map.

- [ ] **Step 1: Add the default-selector regression**

Assert selector `0` produces exactly the same `q`, `pinv`, and statistics as selector `4` on a 9x9 grid. This must fail while default remains RCM.

- [ ] **Step 2: Verify RED**

Run: `make test_ordering`

Expected: default-versus-MLD permutation assertion fails.

- [ ] **Step 3: Switch the default and extend benchmark output**

Dispatch selector `0` to MLD. Extend the demo to report order name, order time, factor time, `nnz(L)`, tree height, fill edges, separator count, and maximum solution error for natural, RCM, minimum degree, and MLD.

- [ ] **Step 4: Update reconstruction documentation**

Document the stable selectors and actual benchmark results. Map recovered `minDegreeOrder*`, `MLDOrder*`, validation, and factor-consumer responsibilities to new source functions and named tests, marking exact-layout/tie-breaking equivalence as unverified.

- [ ] **Step 5: Run final verification**

```bash
make clean
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
./vsdlss_solve 30
make sanitizers
make clean
git diff --check
git status --short
```

Expected: all tests pass, four benchmark rows have finite errors within the documented threshold, sanitizers are clean, and only intended committed files remain.

- [ ] **Step 6: Commit**

```bash
git add README.md src/main.c src/vsdlss_ordering.c test/test_ordering.c test/test_solver.c docs/reconstruction
git commit -m "docs: complete M2 ordering reconstruction"
```

### Task 6: Review and publish the PR update

**Files:**
- Review all files changed from `main`.

**Interfaces:**
- Consumes: green M2 branch.
- Produces: review findings resolved test-first and the updated remote PR branch.

- [ ] **Step 1: Review correctness and evidence claims**

Inspect graph ownership, overflow checks, permutation direction, MLD termination, separator validity, stats equality, and all claims against `reference/`.

- [ ] **Step 2: Convert every finding into a failing regression test**

Run the smallest affected test and observe the expected failure before changing production code.

- [ ] **Step 3: Resolve findings and rerun final verification**

Use the exact Task 5 verification commands and require clean output.

- [ ] **Step 4: Publish to PR #1**

Update `codex/m1-spd-reimplementation` without force and add a PR comment summarizing M2 algorithms, evidence boundary, test commands, and benchmark results.
