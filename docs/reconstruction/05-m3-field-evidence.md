# M3 field and ownership evidence

## Scope and evidence standard

This note records only facts supported by the selected decompilations. It is an
implementation aid, not a C ABI declaration. `int` accesses below are 4 bytes,
`long`, pointers, and `double` accesses are 8 bytes on the recovered binary.
The vector helpers allocate one extra element: an `IVector(n)` occupies
`4*(n+1)` bytes and an `FVector`, `IPVector`, or `FPVector(n)` occupies
`8*(n+1)` bytes. The observed algorithms normally start at element 1.

Confidence labels mean:

- **high**: allocation, access width, and release are all directly observed;
- **medium**: access semantics are directly observed but a name or complete
  lifetime is inferred from multiple callers;
- **unresolved**: the selected evidence does not close the type or lifetime
  chain. Such a field must not be copied into an M3 structure.

The complete functions reviewed were `newFactorBlock`, `deleteFactorBlock`,
`setFactorBlockPtrs`, `newBlockInfo`, `deleteBlockInfo`, `readBlockInfo`,
`writeBlockInfo`, `factorWithinBlock`, `subBlockContribFromRowRange`,
`forwardSolveBlocked`, `backwardSolveBlocked`, `factorDeg1Deg2Pf`, and
`factorDeg3Pf`. Direct allocation/release evidence was checked in
`casiMalloc`, `casiFree`, `newIVector`, `newFVector`, `newIPVector`,
`newFPVector`, and `newLIVector`. The low-degree lifetime cross-check also used
`newDeg1Deg2PfNoDiagVec`, `newDeg1Deg2Pf`, `deleteDeg1Deg2Pf`, `newDeg3Pf`,
and `deleteDeg3Pf`.

## Recovered FactorBlock fields

`newFactorBlock(param_1, param_2, param_3)` allocates a 0x38-byte owner object,
initializes the six scalar words, and allocates four buffers
([`newFactorBlock_vsdlss.c:13-27`](../../reference/funcs/newFactorBlock_vsdlss.c#L13-L27)).
`setFactorBlockPtrs` then writes every scalar at 0x00..0x0c, not merely the
pointer-table entries ([`setFactorBlockPtrs_vsdlss.c:26-35`](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L26-L35)).
The scalar values and their immediate provenance are therefore established;
only their business names remain unresolved.

| Owner type | Offset | Width | Allocator | Writer | Reader | Deallocator | Alias / owned | Confidence |
|---|---:|---:|---|---|---|---|---|---|
| FactorBlock | 0x00 | 4 | inline in 0x38-byte object ([newFactorBlock:13](../../reference/funcs/newFactorBlock_vsdlss.c#L13)) | constructor writes 0 ([line 16](../../reference/funcs/newFactorBlock_vsdlss.c#L16)); `setFactorBlockPtrs` writes inclusive entry count `end-start+1` ([lines 32-34](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L32-L34)) | `setFactorBlockPtrs` uses the computed count as its copy bound ([lines 36-43](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L36-L43)) | object `casiFree` ([deleteFactorBlock:27](../../reference/funcs/deleteFactorBlock_vsdlss.c#L27)) | inline owned scalar | high access/provenance; business name unresolved |
| FactorBlock | 0x04 | 4 | same | constructor writes 0 ([newFactorBlock:17](../../reference/funcs/newFactorBlock_vsdlss.c#L17)); `setFactorBlockPtrs` writes BlockInfo[0x08][block] ([setFactorBlockPtrs:26,30](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L26-L30)) | used to derive 0x00, 0x08, and source-table displacement ([lines 28,32-38](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L28-L38)) | same | inline owned scalar | high access/provenance; semantic role is block start, business name unresolved |
| FactorBlock | 0x08 | 4 | same | constructor writes 0 ([newFactorBlock:18](../../reference/funcs/newFactorBlock_vsdlss.c#L18)); `setFactorBlockPtrs` writes `BlockInfo[0x10][block] + start - 1` ([setFactorBlockPtrs:28,31](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L28-L31)) | used to derive inclusive count ([lines 32-34](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L32-L34)) | same | inline owned scalar | high access/provenance; semantic role is block end, business name unresolved |
| FactorBlock | 0x0c | 4 | same | constructor writes 0 ([newFactorBlock:19](../../reference/funcs/newFactorBlock_vsdlss.c#L19)); `setFactorBlockPtrs` writes BlockInfo[0x18][block] ([setFactorBlockPtrs:29,35](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L29-L35)) | no later read in the selected functions | same | inline owned scalar | high value provenance; business meaning unresolved |
| FactorBlock | 0x10 | 4 | same | constructor writes `param_2` ([newFactorBlock:14](../../reference/funcs/newFactorBlock_vsdlss.c#L14)) | `deleteFactorBlock` does not inspect it; no algorithmic reader in the selected set | same | inline owned scalar | high value provenance; use/meaning unresolved |
| FactorBlock | 0x14 | 4 | same | constructor writes `param_3` ([newFactorBlock:15](../../reference/funcs/newFactorBlock_vsdlss.c#L15)) | `deleteFactorBlock` does not inspect it; no algorithmic reader in the selected set | same | inline owned scalar | high value provenance; use/meaning unresolved |
| FactorBlock | 0x18 | 8 pointer; 4-byte elements | `newIVector(param_2,0,0)` ([newFactorBlock:20-21](../../reference/funcs/newFactorBlock_vsdlss.c#L20-L21)) | allocation installs pointer; element writers not selected | none selected | buffer freed directly ([deleteFactorBlock:11-14](../../reference/funcs/deleteFactorBlock_vsdlss.c#L11-L14)) | FactorBlock owns one contiguous integer buffer | high |
| FactorBlock | 0x20 | 8 pointer; 8-byte elements | `newFVector(0,param_3,0)` ([newFactorBlock:22-23](../../reference/funcs/newFactorBlock_vsdlss.c#L22-L23)) | allocation installs pointer; element writers not selected | none selected | buffer freed directly ([deleteFactorBlock:15-18](../../reference/funcs/deleteFactorBlock_vsdlss.c#L15-L18)) | FactorBlock owns one contiguous double buffer | high |
| FactorBlock | 0x28 | 8 pointer; 8-byte pointer entries | `newIPVector(param_1,1,0)` ([newFactorBlock:24-25](../../reference/funcs/newFactorBlock_vsdlss.c#L24-L25)) | `setFactorBlockPtrs` copies source 0x30 table entries ([setFactorBlockPtrs:22,25,40](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L22-L25)) | copy destination address read by `setFactorBlockPtrs` ([line 25](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L25)) | table freed directly, entries not walked ([deleteFactorBlock:19-22](../../reference/funcs/deleteFactorBlock_vsdlss.c#L19-L22)) | FactorBlock owns table; copied pointees are borrowed aliases | high |
| FactorBlock | 0x30 | 8 pointer; 8-byte pointer entries | `newFPVector(param_1,1,0)` ([newFactorBlock:26-27](../../reference/funcs/newFactorBlock_vsdlss.c#L26-L27)) | `setFactorBlockPtrs` copies source 0x38 table entries ([setFactorBlockPtrs:23-24,41](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L23-L24)) | copy destination address read by `setFactorBlockPtrs` ([line 24](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L24)) | table freed directly, entries not walked ([deleteFactorBlock:23-26](../../reference/funcs/deleteFactorBlock_vsdlss.c#L23-L26)) | FactorBlock owns table; copied pointees are borrowed aliases | high |

Because `param_4` is typed as `int *`, `param_4 + 10` and `param_4 + 0xc`
address byte offsets 0x28 and 0x30. Lines 40-41 copy 8-byte values into those
tables; they do not allocate or clone the pointed-to sequences. The destructor
frees exactly the four top-level buffers and the object, without walking either
pointer table ([`deleteFactorBlock_vsdlss.c:10-27`](../../reference/funcs/deleteFactorBlock_vsdlss.c#L10-L27)).

## Recovered BlockInfo fields

`newBlockInfo(param_1,param_2)` allocates a 0x38-byte owner, stores both scalar
sizes, and allocates six buffers
([`newBlockInfo_vsdlss.c:15-29`](../../reference/funcs/newBlockInfo_vsdlss.c#L15-L29)).
Serialization establishes the buffer extents. In addition,
`setFactorBlockPtrs` reads 0x08 as a per-block start, 0x10 as a per-block
length used to compute an inclusive end, and 0x18 as a copied per-block scalar
([`setFactorBlockPtrs_vsdlss.c:26-35`](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L26-L35)).

| Owner type | Offset | Width | Allocator | Writer | Reader | Deallocator | Alias / owned | Confidence |
|---|---:|---:|---|---|---|---|---|---|
| BlockInfo | 0x00 | 4 | inline in 0x38-byte object | constructor writes `param_1` ([newBlockInfo:15-17](../../reference/funcs/newBlockInfo_vsdlss.c#L15-L17)); deserializer obtains it from header ([readBlockInfo:23-28](../../reference/funcs/readBlockInfo_vsdlss.c#L23-L28)) | serializer reads it and uses it as 0x30 vector extent ([writeBlockInfo:19-25,76-83](../../reference/funcs/writeBlockInfo_vsdlss.c#L19-L25)) | object freed ([deleteBlockInfo:35](../../reference/funcs/deleteBlockInfo_vsdlss.c#L35)) | inline owned scalar | high provenance/extent; business name unresolved |
| BlockInfo | 0x04 | 4 | same | constructor writes `param_2`; deserializer obtains it from header (same citations above) | serializer uses it as extents for 0x08..0x28 ([writeBlockInfo:35-75](../../reference/funcs/writeBlockInfo_vsdlss.c#L35-L75)) | same | inline owned scalar | high provenance/extent; business name unresolved |
| BlockInfo | 0x08 | 8 pointer; 4-byte elements | `newIVector(param_2,0,0)` ([newBlockInfo:18-19](../../reference/funcs/newBlockInfo_vsdlss.c#L18-L19)) | deserializer fills the serialized vector ([readBlockInfo:29-37](../../reference/funcs/readBlockInfo_vsdlss.c#L29-L37)) | serializer reads it ([writeBlockInfo:35-43](../../reference/funcs/writeBlockInfo_vsdlss.c#L35-L43)); `setFactorBlockPtrs` indexes it as block start ([line 26](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L26)) | buffer freed ([deleteBlockInfo:11-14](../../reference/funcs/deleteBlockInfo_vsdlss.c#L11-L14)) | owned contiguous buffer | high; per-block start role established |
| BlockInfo | 0x10 | 8 pointer; 4-byte elements | `newIVector(param_2,0,0)` ([newBlockInfo:20-21](../../reference/funcs/newBlockInfo_vsdlss.c#L20-L21)) | deserializer fills it ([readBlockInfo:38-46](../../reference/funcs/readBlockInfo_vsdlss.c#L38-L46)) | serializer reads it ([writeBlockInfo:45-53](../../reference/funcs/writeBlockInfo_vsdlss.c#L45-L53)); `setFactorBlockPtrs` uses it as length in `start+length-1` ([line 28](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L28)) | buffer freed ([deleteBlockInfo:15-18](../../reference/funcs/deleteBlockInfo_vsdlss.c#L15-L18)) | owned contiguous buffer | high; per-block length role established |
| BlockInfo | 0x18 | 8 pointer; 4-byte elements | `newIVector(param_2,0,0)` ([newBlockInfo:22-23](../../reference/funcs/newBlockInfo_vsdlss.c#L22-L23)) | deserializer fills it ([readBlockInfo:47-55](../../reference/funcs/readBlockInfo_vsdlss.c#L47-L55)) | serializer reads it ([writeBlockInfo:55-63](../../reference/funcs/writeBlockInfo_vsdlss.c#L55-L63)); `setFactorBlockPtrs` copies its indexed value to FactorBlock 0x0c ([lines 29,35](../../reference/funcs/setFactorBlockPtrs_vsdlss.c#L29-L35)) | buffer freed ([deleteBlockInfo:19-22](../../reference/funcs/deleteBlockInfo_vsdlss.c#L19-L22)) | owned contiguous buffer | high access; business meaning unresolved |
| BlockInfo | 0x20 | 8 pointer; 8-byte elements | `newLIVector(param_2,0,0)` ([newBlockInfo:24-25](../../reference/funcs/newBlockInfo_vsdlss.c#L24-L25)) | `readLIVectorNoNewVec` fills it ([readBlockInfo:56](../../reference/funcs/readBlockInfo_vsdlss.c#L56)) | `writeLIVector` serializes it with `param_2` extent ([writeBlockInfo:65](../../reference/funcs/writeBlockInfo_vsdlss.c#L65)) | buffer freed ([deleteBlockInfo:23-26](../../reference/funcs/deleteBlockInfo_vsdlss.c#L23-L26)) | owned contiguous buffer | high width/lifetime; element semantics unresolved |
| BlockInfo | 0x28 | 8 pointer; 4-byte elements | `newIVector(param_2,0,0)` ([newBlockInfo:26-27](../../reference/funcs/newBlockInfo_vsdlss.c#L26-L27)) | deserializer fills it ([readBlockInfo:57-65](../../reference/funcs/readBlockInfo_vsdlss.c#L57-L65)) | serializer reads it ([writeBlockInfo:66-74](../../reference/funcs/writeBlockInfo_vsdlss.c#L66-L74)) | buffer freed ([deleteBlockInfo:27-30](../../reference/funcs/deleteBlockInfo_vsdlss.c#L27-L30)) | owned contiguous buffer | high width/lifetime; element semantics unresolved |
| BlockInfo | 0x30 | 8 pointer; 4-byte elements | `newIVector(param_1,0,0)` ([newBlockInfo:28-29](../../reference/funcs/newBlockInfo_vsdlss.c#L28-L29)) | deserializer fills it ([readBlockInfo:66-74](../../reference/funcs/readBlockInfo_vsdlss.c#L66-L74)) | serializer reads it with `param_1` extent ([writeBlockInfo:76-84](../../reference/funcs/writeBlockInfo_vsdlss.c#L76-L84)) | buffer freed ([deleteBlockInfo:31-34](../../reference/funcs/deleteBlockInfo_vsdlss.c#L31-L34)) | owned contiguous buffer | high width/lifetime; element semantics unresolved |

The serialized header contains the vector payload `{0, param_1, param_2}` under
the observed one-based convention ([`writeBlockInfo_vsdlss.c:22-30`](../../reference/funcs/writeBlockInfo_vsdlss.c#L22-L30)); `readBlockInfo` passes the latter two values to the constructor
([`readBlockInfo_vsdlss.c:23-28`](../../reference/funcs/readBlockInfo_vsdlss.c#L23-L28)). This closes the persistence round-trip shape without assigning unsupported business names.

## Blocked factor view used by numeric routines

The following offsets are repeatedly read from the owner passed to
`factorWithinBlock`, `subBlockContribFromRowRange`, `forwardSolveBlocked`, and
`backwardSolveBlocked`. They form a coherent access view, but the selected
evidence does not establish that owner's constructor or deallocator. They are
listed to prevent accidental conflation with FactorBlock; none is approved as
an M3 field.

| Owner type | Offset | Access width and observed use | Writer / allocator | Reader | Deallocator | Alias / owned | Confidence |
|---|---:|---|---|---|---|---|---|
| blocked-factor owner (identity unresolved) | 0x08 | 4-byte scalar used as terminal/order size by forward/backward solve | unresolved | both solves | unresolved | unresolved | medium access; lifetime unresolved |
| same | 0x14 | 4-byte scalar used as count of block descriptors | unresolved | both solves | unresolved | unresolved | medium access; lifetime unresolved |
| same | 0x20 | 8-byte pointer to 4-byte block-size entries | unresolved | both solves | unresolved | unresolved | medium access; lifetime unresolved |
| same | 0x28 | 8-byte pointer to 4-byte per-column extents | unresolved | all four routines | unresolved | unresolved | medium access; lifetime unresolved |
| same | 0x30 | 8-byte pointer to 8-byte pointers; entries lead to integer row/index sequences | unresolved | all four routines | unresolved | unresolved | medium access; lifetime unresolved |
| same | 0x38 | 8-byte pointer to 8-byte pointers; entries lead to double factor-column storage | unresolved | `factorWithinBlock`, both solves | unresolved | unresolved | medium access; lifetime unresolved |

The 0x28/0x30/0x38 view cannot be the 0x38-byte FactorBlock allocation: it
reads a field at 0x38, one-past that object's allocation, and assigns different
roles to 0x28/0x30. This is direct evidence against treating similarly placed
fields as the same private type.

## Low-degree records and coefficient conversion

The low-degree routines provide algorithmic evidence, while their private
layouts are not suitable for reuse. Degree-1/2 reads its private fields at
[`factorDeg1Deg2Pf_vsdlss.c:38-45`](../../reference/funcs/factorDeg1Deg2Pf_vsdlss.c#L38-L45),
and degree-3 does so at [`factorDeg3Pf_vsdlss.c:35-42`](../../reference/funcs/factorDeg3Pf_vsdlss.c#L35-L42). The following bounded table records the
fields necessary to interpret the coefficient arithmetic.

| Owner type | Offset(s) | Width / role observed | Allocator and writer | Reader | Deallocator | Alias / owned | Confidence |
|---|---:|---|---|---|---|---|---|---|
| Deg1Deg2Pf | 0x04, 0x08 | 4-byte counts used to determine record ranges | `newDeg1Deg2PfNoDiagVec` constructor arguments | `factorDeg1Deg2Pf` | object released by `deleteDeg1Deg2Pf` | inline | high width/provenance |
| Deg1Deg2Pf | 0x10 | pointer to 8-byte diagonal-factor buffer of count sum | `newDeg1Deg2Pf` / `newFVector`; factor writes square roots | `factorDeg1Deg2Pf` writes; later reader not selected | `deleteDeg1Deg2Pf` | owned buffer | high |
| Deg1Deg2Pf | 0x28, 0x30 | first integer-neighbor view and matching double-coefficient view | base buffers allocated by constructor; views assigned there | `factorDeg1Deg2Pf` | base allocations freed at 0x18/0x20 | aliased interior/base views; do not free separately | high lifetime; semantic names medium |
| Deg1Deg2Pf | 0x38, 0x40 | second integer-neighbor and coefficient interior views | derived from the same base buffers in constructor | `factorDeg1Deg2Pf` | base allocations freed at 0x18/0x20 | borrowed interior aliases | high lifetime; semantic names medium |
| Deg3Pf | 0x04 | 4-byte record count | constructor argument | `factorDeg3Pf` | object `casiFree` | inline | high |
| Deg3Pf | 0x08 | pointer to 8-byte diagonal-factor buffer | `newFVector`; factor writes square roots | `factorDeg3Pf` | `deleteDeg3Pf` | owned buffer | high |
| Deg3Pf | 0x10/0x18, 0x20/0x28, 0x30/0x38 | three pairs of integer-neighbor and double-coefficient buffers | `newIVector`/`newFVector`; matrix formation writes; factor normalizes coefficients | `factorDeg3Pf` | `deleteDeg3Pf` frees each buffer | six independently owned buffers | high |

For an eliminated vertex with current diagonal pivot `d > 0` and coupling
`a` to a remaining neighbor, the original code first performs Schur updates
using `a*a/d` and `a_i*a_j/d` for fill
([`factorDeg1Deg2Pf_vsdlss.c:82-100`](../../reference/funcs/factorDeg1Deg2Pf_vsdlss.c#L82-L100);
[`factorDeg3Pf_vsdlss.c:68-105`](../../reference/funcs/factorDeg3Pf_vsdlss.c#L68-L105)),
writes `sqrt(d)` to its diagonal factor buffer, and then divides each coupling
by `sqrt(d)` ([`factorDeg1Deg2Pf_vsdlss.c:158-198`](../../reference/funcs/factorDeg1Deg2Pf_vsdlss.c#L158-L198);
[`factorDeg3Pf_vsdlss.c:59-70`](../../reference/funcs/factorDeg3Pf_vsdlss.c#L59-L70)). Thus its stored
factor coefficient is

```
L_neighbor,vertex = a / sqrt(d),    L_vertex,vertex = sqrt(d).
```

M3's reduction record instead stores the elimination multiplier

```
m = a / d = L_neighbor,vertex / L_vertex,vertex.
```

These representations are mathematically convertible when `d > 0`, but they
are not element-for-element storage equivalents. M3 recovery can use
`x_vertex = (saved_rhs - sum(a_i*x_i))/d`, or equivalently retain `m_i` plus
the data needed by its chosen recovery formula. No private low-degree offsets,
one-based sentinels, or square-root coefficient buffers are required by the
independent implementation.

## M3 ownership contract derived from the evidence

1. Every M3 public constructor sets its output to `NULL` before allocation and
   publishes an object only after complete construction.
2. Every allocation has one owning object and one release path. Interior views
   and pointer-table entries are explicit borrows and are never freed.
3. A pointer table and its pointees have separate ownership declarations. The
   FactorBlock evidence specifically forbids inferring pointee ownership from
   ownership of the table.
4. M3 copies any symbolic layout needed after analysis into the numeric factor;
   it does not retain a dangling borrow from a symbolic owner.
5. RHS-specific arrays (`saved`, reduced RHS, solve workspace, and candidate
   solution) are solve-call storage. They are not owned by the reusable factor.
6. Failed construction frees every allocation already owned. Failed solve does
   not publish a partial solution.
7. The private offsets and all rows marked unresolved above are documentary
   evidence only. They do not enter M3 declarations, casts, serialization, or
   runtime dependencies.

## Creation/use/free audit

| New M3 concept | Creation | Use | Release / ownership boundary |
|---|---|---|---|
| elimination record array | reduction constructor allocates checked `count * sizeof(record)` | factor-independent RHS reduction and reverse recovery | reduction owner frees it once |
| core vertex map | reduction constructor allocates checked `core_n * sizeof(csi)` | maps core order to component order | reduction owner frees it once |
| reduced core matrix | reduction constructor builds a new matrix | symbolic/numeric core factorization | reduction owner frees it once |
| symbolic block layout | analyze constructs checked arrays | factorization reads it | symbolic destructor frees it; numeric factor must own a copy of retained layout |
| numeric supernodal factor | factorize allocates checked block/index/value storage | repeated solves and optional export | factor destructor frees all owned buffers; no RHS ownership |
| exported `L` | export allocates a new CSC | caller diagnostics/comparison | caller owns and frees it |
| per-solve workspace and saved RHS | solve path allocates or receives independent arrays | one RHS transaction | released at end of call; failure leaves caller solution unchanged |

This audit states M3's intended ownership boundary; it does not claim that the
original private structures have been fully reconstructed.
