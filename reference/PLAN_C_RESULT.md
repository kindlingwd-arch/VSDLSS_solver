# Plan C — "can the decompiled code actually solve?" (empirical verdict)

Attempted to drive the **decompiled** dense solver
(`denseSolve_vsdlss` → `newMatrix` → `convertElemArrToDense` →
`denseCholesky` → `solveDenseLower/Upper`) with an in-memory matrix.

## What worked (with pointer-truncation fix)
- Recompiling the dense path with **exact prototypes** (`dense_fix.h`) fixed the
  biggest ABI bug: without a prototype, pointer-returning calls were compiled as
  `int` (`movslq %eax`), truncating 64-bit returns to 32 bits.
- `newMatrix`, `newVector_vsdlss`, `convertElemArrToDense` then ran and produced
  a correct dense matrix from a VSDLSS `ElemArr` (1-based node indices,
  polynomial-vector layout: element `i` at byte offset `8*(i+1)`).

## Where it definitively stops
`denseCholesky` (the numeric factorization kernel). The decompiler **lost the
numeric/type info**, so its C is corrupted and cannot run:
```c
saxpy_vsdlss((ulong)(*(double *)(lVar4+8)/__x) ^ DAT_00abd200, ...);   // double XOR a global
scaleVector_vsdlss(_DAT_00ab67f8 / dVar8, ...);                        // uninitialized global / d
...
lVar2 = lVar3 + *(long *)(param_1 + 8 + lVar3);                        // broken row offset
```
These produce garbage multipliers and polynomial-vector offsets that write
out of bounds (row[n+1]), causing a segfault. This is **not fixable by shims** —
the underlying double/global/numeric-type information was lost during
decompilation of the factor kernel.

## Conclusion
- ✅ The decompiled code **can be compiled, linked, loaded and executed**.
- ❌ It **cannot be made to solve correctly**: the Cholesky/LU numeric kernels
  (`denseCholesky`, `saxpy_vsdlss`) were decompiled with corrupted arithmetic
  (types lost), so the recompiled code computes garbage / crashes.
- This is the irreducible boundary of "recompiling Ghidra output". The reliable
  way to get a working solver is `src/vsdlss.c` (faithful re-implementation,
  already compiling & solving) or the pristine `asim_3d` binary.

Artifacts: `reference/dense_fix.h` (exact dense-path prototypes),
`reference/libvsdlss_rebuilt.so` (assembled closure) and
`reference/libdense.so`-style minimal dense build (in WSL build scripts).
