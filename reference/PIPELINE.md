# VSDLSS run pipeline (reverse-engineered data flow)

## Entry
`solverVsdl64(int argc, char **argv)`  (@0x0095fe30, see `harness/solverVsdl64.c`)
- CLI options: `-S` silent, `-U/-u` unref, `-W/-w` write, `-p <n>` order-selector,
  `-s` single-solve.
- Remaining positional arg = **job name**; forms `<jobName>.CLG`, `fopen`s it
  (`LMBKdFIBkyFKYrXSPAIPP`), and dispatches:
  - no `-s`  → `JyFKkyQXSPAIPP(jobName, orderSel, writeFlag)`  (@0x0095d520)
  - with `-s`→ `JyFKqMF_BXSPAIPP(jobName, orderSel, &flag)`      (@0x0095eff0)

## Job file format (`.CLG`) — keyword-driven text
The driver tokenizes the file with a parser keyed on the directive set
`nomardcpslkxuzh` / `NOMARDCPSLKXUZH`. Each uppercase letter is a section
keyword (`N`, `O`, `M`, `A`, `R`, `D`, `C`, `P`, `S`, `L`, `K`, `X`, `U`, `Z`,
`H`) read by `DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP`, and drives construction of the
matrix/RHS.  The parser is the large `harness/driver/JyFKkyQXSPAIPP.c`
(844 lines decompiled).

## Prepare matrix + RHS
From the parsed job, the flow calls:
- `readSpiceAndWriteMatVecs_vsdlss`  (@0x009d43e0)  — parse netlist elements
- `writeMatVecsFile_vsdlss`          (@0x009d3c20)  — write matrix-vector files
- `writeIns_vsdlss` / `writeRhsVec_vsdlss` / `readSpiceAndWriteMatVecs_vsdlss`

which materialize the sparse matrix (CSR) and RHS into the solver's disk
format (the "chunked" file store).

## Solve (the core `vsdlss`)
`vsdlss` (@0x009dad10, 37-arg contract in `harness_contract.h`) then:
1. self-init: `setNumThreadsInBlasLib`, `initCasiMutexes`, `initTimerPool`
2. order selection: `enqOrderSelector_vsdlss` (min-degree / multilevel nesting)
3. symbolic elimination + numeric factor: `factorA_vsdlss`
4. solve: `solveLoadCase_vsdlss` (`forwardSubstitute` / `backSubstitute`)
5. cleanup: `removeAllChunkFiles_vsdlss` / `cleanUpMemory_vsdlss`
All the `_vsdlss` functions are in `reference/funcs/` (688 decompiled, 665 compile).

## Init environment the solver expects
- memory manager: `initMemManager_vsdlss` + `casiMalloc_casi_vsdlss` /
  `casiFree_casi_vsdlss` (or the app-supplied `appMalloc/appFree` fn-ptrs
  passed as vsdlss args 20/21)
- error handling: `setjmp`/`longjmp` via `vsdlssEnv_vsdlss` /
  `vsdlssStatus_vsdlss`
- chunked-file store: `initFManager`, `initChunkWrite/Read`,
  `openFactorChunkFileW`, ... (the solver's on-disk factor store)
- threading/mutexes: `initErrorMutex`, `initRandPermMutex`

## Files in this folder
- `harness/`          decompiled C++ harness + matrix wrapper classes
- `harness/driver/`   decompiled obfuscated driver/parser layer (675 funcs)
- `harness_contract.h` rebuilt calling interface
- `funcs/`            the 688 `_vsdlss` solver functions
- `types/`            recovered data-type manager (2847 types)
