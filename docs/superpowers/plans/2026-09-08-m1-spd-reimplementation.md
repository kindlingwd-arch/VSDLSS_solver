# M1 Trusted SPD Solver Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the current loosely validated SPD path with a deterministic, reusable, and testable in-memory sparse Cholesky solver that satisfies M1.

**Architecture:** Normalize every caller-owned upper-triangular CSC matrix into an owned canonical matrix before ordering or factorization. Keep matrix validation/normalization, ordering, numerical factorization, solve state, file I/O, and tests in separate modules, while retaining legacy boolean wrappers over a status-returning public API.

**Tech Stack:** C11, GNU Make, Python 3 test-data utilities, AddressSanitizer/UndefinedBehaviorSanitizer.

**Spec:** `docs/reconstruction/03-design-and-roadmap.md`

## Global Constraints

- M1 supports real double, square symmetric positive-definite matrices only.
- Public indices are `int64_t`; input is 0-based upper-triangular CSC.
- Input may be unsorted and contain duplicates; normalization sorts and sums duplicates without modifying the input.
- `q[new] = old` and `pinv[old] = new` everywhere.
- Orders 0 and 1 mean RCM; order 2 means natural order; all others return unsupported.
- Factor construction owns normalized storage and permutation and permits sequential repeated RHS solves.
- Failed in-place solves leave the caller's RHS unchanged.
- NaN, infinity, malformed CSC, lower-triangular entries, empty matrices, overflow, and I/O corruption return explicit errors.
- M1 does not claim LU, LDLT, MLD, out-of-core operation, concurrency, or original ABI compatibility.

---

### Task 1: Test runner and public status contract

**Files:**
- Modify: `include/vsdlss.h`
- Create: `test/test_solver.c`
- Modify: `Makefile`

**Interfaces:**
- Produces: `vsdlss_status`, `vsdlss_status_string(vsdlss_status)`, and a test executable that returns nonzero on any failed assertion.

- [ ] Write a failing test that expects distinct strings for `VSDLSS_OK`, `VSDLSS_ERR_INVALID`, `VSDLSS_ERR_OOM`, `VSDLSS_ERR_NOT_POSDEF`, `VSDLSS_ERR_NONFINITE`, `VSDLSS_ERR_IO`, and `VSDLSS_ERR_UNSUPPORTED`.
- [ ] Run `make test-unit`; expect compilation to fail because the status API is absent.
- [ ] Add the enum and declaration to `include/vsdlss.h`, implement the string mapping, and add `test-unit`, `test`, and sanitizer targets whose recipes build every executable they run.
- [ ] Run `make test-unit`; expect all status assertions to pass.

### Task 2: Checked matrix validation and canonicalization

**Files:**
- Create: `src/vsdlss_matrix.c`
- Modify: `include/vsdlss.h`
- Modify: `Makefile`
- Test: `test/test_solver.c`

**Interfaces:**
- Produces: `vsdlss_validate_upper_csc(const vsdlss *)`, `vsdlss_normalize_upper(const vsdlss *, vsdlss **out)`, `vsdlss_spmv_sym_upper(const vsdlss *, const double *, double *)`, and `vsdlss_backward_error(...)`.

- [ ] Add tests for valid unsorted duplicates, duplicate summation, source immutability, lower entries, nonmonotone pointers, row overflow, null arrays, NaN/Inf, empty/nonsquare matrices, and allocation-size overflow.
- [ ] Run `make test-unit`; expect failures at the first missing matrix function.
- [ ] Implement checked size multiplication, structural validation before array traversal, per-column sorting, duplicate summation, finite checking, symmetric SpMV, and infinity-norm backward error.
- [ ] Run `make test-unit`; expect matrix tests to pass.

### Task 3: Ordering contract and high-degree-safe RCM

**Files:**
- Create: `src/vsdlss_ordering.c`
- Modify: `src/vsdlss.c`
- Modify: `include/vsdlss.h`
- Test: `test/test_solver.c`

**Interfaces:**
- Produces: `vsdlss_order(const vsdlss *, int, csi **q, csi **pinv)` and `vsdlss_validate_permutation(const csi *, const csi *, csi)`.

- [ ] Add tests for natural order, unsupported order, non-self-inverse graph permutation, disconnected components, isolated nodes, and the 1026-node highest-numbered-center star.
- [ ] Run `make test-unit`; expect the star/permutation tests to fail against the existing 512-entry neighbor buffer and old direction contract.
- [ ] Move ordering out of `src/vsdlss.c`, use dynamically sized adjacency and neighbor workspace, sort each BFS frontier by `(degree,node)`, reverse each connected component, then derive `pinv[old]=new` from `q[new]=old`.
- [ ] Run `make test-unit`; expect every permutation property test to pass under sanitizers.

### Task 4: Reusable factor and transactional solves

**Files:**
- Modify: `include/vsdlss.h`
- Modify: `src/vsdlss.c`
- Test: `test/test_solver.c`

**Interfaces:**
- Produces: opaque `vsdlss_factor`, `vsdlss_factorize(const vsdlss *, int, vsdlss_factor **)`, `vsdlss_factor_solve(const vsdlss_factor *, const double *, double *)`, `vsdlss_factor_free(vsdlss_factor *)`, and compatibility wrappers.

- [ ] Add tests for the 2x2 exact example, alternating-sign tridiagonal and Poisson solutions, both orders, three RHS on one factor, unchanged factor, unchanged output on failure, and rejection of indefinite, singular, and nonfinite inputs.
- [ ] Run `make test-unit`; expect compilation failures for the new factor API.
- [ ] Make factorization normalize first, obtain `q/pinv`, build `PAP^T`, perform symbolic/numeric Cholesky with finite/nonpositive-pivot checks, and retain owned factor/permutations. Solve through a temporary vector and copy to output only after all checks pass.
- [ ] Run `make test-unit`; expect solution error and backward error thresholds from the spec to pass.

### Task 5: Factor reconstruction and robustness tests

**Files:**
- Modify: `include/vsdlss.h`
- Modify: `src/vsdlss.c`
- Test: `test/test_solver.c`

**Interfaces:**
- Produces: read-only factor inspection for `L`, `q`, and `pinv`, sufficient for tests without exposing ownership.

- [ ] Add independent dense reconstruction of `PAP^T` and `LL^T` for small matrices and verify relative infinity error at most `1e-12`.
- [ ] Run `make test-unit`; expect failure until inspection is available and permutation direction is exact.
- [ ] Add const accessors and strengthen all symbolic capacity, allocation, reach, diagonal, and triangular-solve checks.
- [ ] Run `make sanitizers`; expect zero sanitizer diagnostics and all assertions passing.

### Task 6: Strict RedHawk file adapter and CLI

**Files:**
- Create: `src/vsdlss_io.c`
- Modify: `include/vsdlss.h`
- Modify: `src/main_vsdlss.c`
- Modify: `src/vsdlss_catalog.c`
- Modify: `src/vsdlss_harness.c`
- Test: `test/test_io.c`
- Modify: `Makefile`

**Interfaces:**
- Produces: `vsdlss_load_job(const char *, int allow_missing_rhs, vsdlss **A, double **rhs)` and `vsdlss_write_solution(const char *, const double *, csi)`.

- [ ] Add temporary-directory tests for valid files, missing required files, short files, invalid 1-based coordinates, a missing RHS in strict/demo modes, a short existing RHS, and output-open/write failures.
- [ ] Run `make test-io`; expect failures against the current exit-based and silent-fallback loaders.
- [ ] Implement a status-returning adapter with exact-length checks, explicit little/native file-width handling, canonicalization through the matrix layer, and explicit demo-mode RHS synthesis. Route both CLIs through it and remove duplicate loaders.
- [ ] Run `make test`; expect unit and I/O suites plus the generated sparse smoke test to pass.

### Task 7: Compatibility cleanup, documentation, and final verification

**Approved scope amendment (2026-09-08):** Delete the old compatibility/catalog/MLD/harness sources instead of repairing them. Preserve `reference/` as evidence and compile only the new M1 implementation.

**Files:**
- Modify: `include/vsdlss_api.h`
- Modify: `include/vsdlss_catalog.h`
- Modify: `src/vsdlss_api.c`
- Modify: `src/vsdlss_catalog.c`
- Modify: `docs/reconstruction/CONTINUATION.md`
- Modify: `docs/reconstruction/02-existing-code-audit.md`
- Modify: `README.md`
- Modify: `Makefile`

**Interfaces:**
- Produces: one canonical factor type and explicit unsupported returns for MLD/block/ABI placeholders.

- [ ] Add a compile/link compatibility test that includes each public header in isolation and together.
- [ ] Run it; expect failure from the current conflicting `factorA_vsdlss` declarations.
- [ ] Route legacy functions through the canonical API, replace pointer-truncating fake block types, and ensure unavailable features cannot silently report success.
- [ ] Run `make clean && make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' all solver harness test-unit test-io`.
- [ ] Run `make sanitizers` and the nonconstant Poisson/tridiagonal benchmarks; record commands, exit codes, residuals, backward errors, and remaining M2+ limitations in the reconstruction documents.
- [ ] Review the diff against every M1 acceptance row, then commit the verified implementation.
