> 当前重建入口：[原理、审计与续接文档](docs/reconstruction/README.md)。
> 以下 README 保留原项目的历史说明；其中功能完成度和测试结果尚未在本轮重新验证，以 `docs/reconstruction` 的审计结论为准。
> 云端继续开发时，先阅读 [CONTINUATION.md](docs/reconstruction/CONTINUATION.md)。仓库根目录即原本的 `vsdlss_solver` 项目目录。

# VSDLSS — Very Sparse Direct Linear System Solver (reconstructed)

This directory contains the outcome of reverse-engineering the **`vsdlss`**
(Very Sparse Direct Linear System Solver) functions from the
`asim_3d` program shipped in the RedHawk-SC solver (`RH-lite/bin/asim_3d`,
ELF x86-64, 8.6 MB, symbols kept in `.dynsym`).

## 1. What was done (Ghidra 12.1.3, headless in WSL)

1. Imported and analyzed `asim_3d` (full auto-analysis, 30+ analyzers).
2. Located the top-level solver entries via the preserved dynamic symbols:
   - `vsdlss` @ `0x009dad10` (8279 bytes — driver in `vsdlss.c`)
   - `solverVsdlss` @ `0x009d7620`
   - `vsdlss1_vsdlss` @ `0x00a1b110`, `vsdlss1Mem_vsdlss` @ `0x00a13000`
   - C++ wrappers `CasiSolver<int>::run_vsdlss_` / `setup_vsdlss_` @ `0x005fd...`
3. **Decompiled EVERY symbol whose name contains `vsdlss`** — **688 functions**
   (all 302 `_vsdlss` FUNC symbols + the C++ wrappers) into
   `reference/funcs/*.c`.  Each file is the faithful Ghidra decompiler output
   (header comment with name/address/size + signature + decompiled C body).

## 2. Reference artifact (the hundreds of dependency functions)

```
reference/funcs/               688 decompiled .c files (one per function)
reference/index.txt            address / size / name index
reference/vsdlss_ref.h         Ghidra type shims + standard-header fixes
reference/vsdlss_globals.h     extern decls for Ghidra global/DAT labels
reference/vsdlss_funcdecl.h    recovered C function declarations
reference/vsdlss_globals.txt   global-variable list
reference/types/vsdlss_types.h  Ghidra recovered data-type manager (C view)
reference/vsdlss_prototypes.h  recovered function prototypes (real types)
reference/vsdlss_extern.h      external-library symbol declarations
reference/vsdlss_stub.c        weak stubs for the externals
reference/vsdlss_link.txt      DT_NEEDED libs + suggested -l flags
reference/libvsdlss_ref.a      archive of the reference functions that compile
```

The original name strings reveal the algorithm family:
`* VSDLSS 4.4: Very Sparse Direct Linear System Solver` and hundreds of
`_vsdlss` helpers (`MLD*` multilevel nested-dissection ordering,
`minDegreeOrder`, `symbolicElimination`, `factorA`, `forwardSubstitute`,
`backSubstitute`, `denseCholesky`, `solveDenseLower/Upper`).

### Consistency / compilability measure

With the shim + global + macro fixes, **665 of the 688 decompiled functions
pass a C compile check (96.7%)** and were archived into `libvsdlss_ref.a`
(1.5 MB).  The remaining 23 need genuine data-model/type backfill:

- **tcmalloc wrappers** (`anilFree`, `anilMalloc`, `encapsulatedFree/Malloc`):
  they call `tcmalloc::ThreadCache::...` — C++ with no C representation.
- **Typed-pointer globals** (`initChunkWrite`, `newCEdgelist`, `newBlock`,
  `newCharPtrList`, `mainMat`, `printError/Stats/Warning`, `init/finish
  ReadFromTo`, `allocatePf*`, `getSearchIndices`): the decompiled code
  dereferences Ghidra globals as typed pointers.  Fix by declaring them with
  the right base type (e.g. `extern undefined4 *cmuFromNodeVecWrite;`).  A
  starting scaffold is in `reference/vsdlss_structs.h`.

Steps (documented in `reference/vsdlss_structs.h`): add the affected global
names to the exclude list of `gen_globals.py`, then declare them with the base
type seen at their cast site.

## 3. Recovered data model & harness (what Ghidra found)

`ExportTypes.java` exported the whole `DataTypeManager` and the recovered
function prototypes. The prototypes reveal the real call harness:
`CasiSolver<int>::setup_vsdlss_(CasiSolver<int>*, Symmetric*)` and
`run_vsdlss_(CasiSolver<int>*, SolveCase, double*)`, and the matrix object
type `Symmetric`.  These are the pieces needed to reconstruct the **caller
harness** that drives the 37-parameter `vsdlss` entry.

### Step 2 — the harness interface (recovered)

`reference/harness/` + `reference/harness_index.txt` are the decompiled C++
harness functions. Key findings:

- The top-level entry is **`solverVsdl64(int argc, char **argv)`**
  (@0x0095fe30): a mini-`main` that parses CLI options (`-S` silent,
  `-U/-u` unref, `-W/-w` write, `-p <n>` order-selector, `-s` single-solve)
  followed by a **job name**, forms `<jobName>.CLG`, opens it and dispatches
  to the obfuscated driver (`JyFKkyQXSPAIPP` / `JyFKqMF_BXSPAIPP`).
- `CasiSolver<long>` layout: vtable (@0x00) + `std::string` jobname
  (@0x08/0x10/0x18) + `Symmetric*` matrix (@0x28) + rhs pointer (@0x30).
- `CasiSolver<int>::run_vsdlss_` builds a 0x30-byte control struct and calls
  `solverVsdl64(5, &jobname)`.
- The 37-argument `vsdlss` contract is documented in
  `reference/harness_contract.h` (jobname, nElems, reorder/mode, reduce flag,
  app malloc/free pointers, etc.).

`src/harness.c` reproduces the `solverVsdl64` CLI contract so a driver can be
built against it.

> To actually *run* the decompiled solver end-to-end, the `.CLG` **job-file
> format** and the solver's **init environment** (memory manager, chunked-file
> store, `setjmp` error jump) must also be reproduced — that is step 3.

### Step 3 — run pipeline & job-file contract (mapped)

The complete data flow is now located/decompiled (see `reference/PIPELINE.md`):
`jobName.CLG` (keyword format, directives `NOMARDCPSLKXUZH`) → driver
`JyFKkyQXSPAIPP` parses it → matrix/RHS written by
`readSpiceAndWriteMatVecs_vsdlss`/`writeMatVecsFile_vsdlss` into the solver's
chunked file store → `vsdlss` (37-arg contract) self-initializes
(memory manager, mutexes/timers, chunked-file store) → factor (`factorA_vsdlss`)
→ solve (`solveLoadCase_vsdlss`) → cleanup.  The decompiled driver/parser layer
is in `reference/harness/driver/`.

To run the decompiled solver end-to-end, this pipeline must be reproduced
(the keyword parser + solver file store).  The pristine `asim_3d` binary already
implements it; with the shipped libs it can be executed directly as the
authentic baseline.

## 4. External dependencies (MKL / CUDA / tcmalloc / Intel runtime)

The RedHawk install ships the full runtime:
`RH-lite/lib/mkl/*` (MKL core/lp64/sequential/vml), `RH-lite/lib/mf2/*`
(CUDA cublas/cudart, Intel-comp ifcoremt/imf/intlc/iomp5/irc/svml),
`RH-lite/lib/*` (nffr/nffw/nsys/ngcore/ansmf, Qt, stdc++), `RH-lite/lib/ssl/*`
(aclanci/ssl/crypto), `tcl8.6`, `zlib`.  All are declared (weak) in
`vsdlss_extern.h`; `vsdlss_stub.c` provides weak stubs; `vsdlss_link.txt`
lists the real link flags and DT_NEEDED libraries.

Because the entire runtime ships, **the original `asim_3d` executable can be
run directly with these libraries** (set `LD_LIBRARY_PATH`) — the most faithful
way to execute the genuine VSDLSS.

## 5. Runnable solver (self-contained)

The decompiled vendor code is C++ with templates and depends on the full closed
memory-manager / chunked-file / threading infra plus MKL/CUDA. So `src/`
provides a **faithful, self-contained, compilable re-implementation** of the
same sparse-direct-solve algorithm (derived from CSparse, LGPL-2.1+):

| real VSDLSS step        | this project                        |
|-------------------------|-------------------------------------|
| ordering (RCM / min-deg)| `vsdlss_rcm` (RCM) / identity       |
| symbolic analysis       | `vsdlss_etree`, `vsdlss_cholcounts` |
| numeric factorization   | `vsdlss_chol` (sparse LLᵀ)          |
| forward substitution    | `vsdlss_lsolve`                      |
| backward substitution   | `vsdlss_ltsolve`                     |

```bash
make                 # produces ./vsdlss_solve
./vsdlss_solve 30     # 2-D Poisson 30x30 (n=900), b = A*ones
```

Both orderings recover the exact answer (`x = ones`) to ~1e-15:
```
dimension n = 900, nnz(A upper) = 2640, grid = 30x30
[natural] ok=1  max|solution-1| = 5.773e-15
[RCM    ] ok=1  max|solution-1| = 2.465e-14
```

## 5b. File-format harness (reads real RedHawk `.hdr`/`.mat*`)

`src/vsdlss_harness.c` reads the **actual VSDLSS input format** (the same files
the decompiled `mainMat_vsdlss` parses) and solves with the reconstructed
solver:

- `.hdr`  : text `KEY VALUE` (`n` rows, `o` off-diagonals, `m a k z r c x u`)
- `.matd` : binary `n` doubles (diagonal)
- `.matf` / `.matt` : binary `o` int32 (row/col, 1-based)
- `.mato` : binary `o` doubles (off-diagonal values)
- `.rhs`  : binary `n` doubles (optional; if absent, `b = A·ones`)

```bash
make harness            # builds ./vsdlss_harness
python3 test/gen_sparse.py test_sparse 10   # generate a sample SPD input
./vsdlss_harness test_sparse                # reads files, solves A x = b
make test                                   # build + generate + run self-test
```

Verified on the generated tridiagonal SPD matrix: `||A x - b||_inf ≈ 2e-16`,
`x = ones`.

## 5c. Reproduced VSDLSS solver (function catalog, from the decompiled code)

`src/vsdlss_catalog.c` mirrors the **function / feature catalog** of the
decompiled `vsdlss` solver — the same set of functions it needs, each with our
own correct implementation (same names and responsibilities, not a verbatim
copy of the corrupted decompiler output):

| decompiled function          | our implementation                    |
|------------------------------|---------------------------------------|
| `newIVector_vsdlss`, `newFVector_vsdlss`, `initVector_vsdlss`, `saxpy_vsdlss`, `innerProduct_vsdlss` | vector utilities |
| `denseCholesky`, `solveDenseLower`, `solveDenseUpper` | dense helpers |
| `minDegreeOrder`, `vsdlss_rcm` | fill-reducing ordering |
| `newFactorBlockHeader_vsdlss`, `newFactorChunk_vsdlss` | factor block/chunk data model |
| `permuteIVector_vsdlss`, `permuteDVector_vsdlss`, `permuteDVectorInPlace_vsdlss`, `permuteFVector_vsdlss`, `permuteIPVector_vsdlss`, `permuteIVectorInPlace_vsdlss`, `invPermuteDVector_vsdlss`, `infNorm_vsdlss`, `oneNorm_vsdlss`, `twoNorm_vsdlss`, `DNorm_vsdlss` | vector permutation / norms |
| `copyVector_vsdlss`, `copyFVector_vsdlss`, `copyIVector_vsdlss`, `addVectors_vsdlss`, `subVectors_vsdlss`, `addIVectors_vsdlss`, `scaleVector_vsdlss`, `scaleVectorDiag_vsdlss`, `sqrtDVector_vsdlss`, `sqrtFVector_vsdlss`, `sumIVectorElems_vsdlss`, `getMinIVector_vsdlss`, `getMaxIVector_vsdlss`, `copyStrArray_vsdlss` | vector math |
| `mainMat_vsdlss`, `readMatrixFromFile_vsdlss`, `checkForNegativeDiags_vsdlss`, `checkResults_vsdlss`, `writeRhsVec_vsdlss`, `writeNodalVoltages_vsdlss` | matrix construction / I-O / validation |
| `factorA_vsdlss` | sparse Cholesky factor |
| `forwardSubstitute_vsdlss`, `backSubstitute_vsdlss`, `solveWithBlockLower`, `solveWithBlockUpper`, `solveLoadCase_vsdlss` | triangular / load-case solves |
| `forwardSolveBlocked_vsdlss`, `backwardSolveBlocked_vsdlss`, `solveLoadCaseBlocked_vsdlss`, `forwardSolveFromVsdlss`, `backwardSolveFromVsdlss`, `forwardBackWardSolveFromVsdlss` | blocked / from-solve methods |
| `preSolve_vsdlss`, `postSolve_vsdlss`, `solveLowerTriangPartFact_vsdlss`, `solveUpperTriangPartFact_vsdlss`, `solveLowerTriangForPartFact_vsdlss` | pre/post + part-factor solves |
| `getNonZerosInFactLForVsdlss`, `getFactorStats_vsdlss`, `printStats_vsdlss`, `printFloatArray_vsdlss`, `printFVectorToLog_vsdlss` | factor statistics |
| `casiTimes_vsdlss`, `initTime_vsdlss`, `newTime_vsdlss`, `deleteTime_vsdlss`, `copyTimes_vsdlss`, `addTimes_vsdlss`, `diffTimes_vsdlss`, `scaleTime_vsdlss`, `accumDiffTimes_vsdlss`, `convertToDeciSeconds_vsdlss`, `convertToCentiSeconds_vsdlss`, `printTimeDeciSeconds_vsdlss`, `printDiffTimes_vsdlss` | timing / solve-speed reporting |
| `solverVsdlss` | CLI/file entry (reads `.hdr`+`.mat`+`.rhs`, writes `.rsl`) |

```bash
make solver             # builds ./vsdlss_solver
./vsdlss_solver test_sparse    # reads files, solves, writes <job>.rsl
```

Verified: `x = ones` (n=10, n=500), machine precision; exports the real
decompiled symbols (`factorA_vsdlss`, `solveLoadCase_vsdlss`, `minDegreeOrder`,
`denseCholesky`, `solverVsdlss`, ...).

## 5d. Data-structure utilities (`vsdlss_ds.c`)

`src/vsdlss_ds.c` + `include/vsdlss_ds.h` reproduce the VSDLSS *container /
ordering* utilities with correct, self-contained implementations (same names and
responsibilities as the decompiled code, opaque `longlong` handles):

- **Priority queues** — `Pq` (min-heap on node indices), `Pqi` (indexed, with a
  `pos` array), `Bpq` (bucket PQ by integer key): `newPq`, `insertPqInt`,
  `minPq`, `deqPq`, `updatePq`, `flushPq`, `moreInPq`, `elemInPq`, ...
- **Stack / deque** — `newStack`, `pushAStack`, `popAStack`, `newDoubleEndedQ`,
  `nq`, `dq`, ...
- **Lists / marks** — char-ptr list, `CEdgelist` (edge list), `ListMark` /
  `CountingMark` (mark arrays).
- **Hash table** — open-addressing `newHashTable`, `insertIntoHt`,
  `deleteBucketFromHt`, `hashBucketEmpty`.
- **Adjacency graph** — `newGraph` / `readGraph` / `writeGraph`, convert
  (`G2GLT`, `G2GUT`, `GLT2GUT`, `GUT2G`), eliminate duplicate edges, renaming.
- **Multilevel nested dissection** — `MLDOrder` builds the symmetric adjacency
  from the matrix and runs a genuine recursive-bisection nested-dissection to
  produce a fill-reducing permutation (falls back to identity if no matrix
  handle is given); full `MLD*` skeleton family (`MLDCoarsen`, `MLDFindPartition`,
  `MLDBalance`, `MLDRMMatching`, `MLDUncoarsenRefine`, ...).

```bash
make ds                 # builds + runs ./vsdlss_ds_test (container self-test)
```

`vsdlss_ds_test` passes (`ds_test: ALL OK`) and the module compiles clean
(`-Wall -Wextra`) and links into `vsdlss_solver`.

## 6. Limitations

- Reference decompile is faithful Ghidra output; 24 of 688 functions still need
  the recovered struct data-models / tcmalloc types to compile.
- The runnable solver handles **SPD** systems via sparse Cholesky; the real
  VSDLSS also supports general LU and out-of-core.
- Ordering is RCM (simple); the original uses nested dissection + minimum-degree.
  A genuine `MLDOrder` (recursive-bisection nested dissection) is now provided by
  `vsdlss_ds.c`; the solver currently drives the simple RCM/identity path.
- Compiling then *running* the decompiled original additionally requires
  reconstructing the caller harness (`CasiSolver` / `Symmetric` / `SolveCase`,
  documented in `vsdlss_prototypes.h`) and reproducing the solver's global
  memory-manager / chunked-file initialization environment.
