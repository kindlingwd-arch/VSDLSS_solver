/* harness: cColumnNormEnforcer addr=0090ccd0 size=658 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* cColumnNormEnforcer::cColumnNormEnforcer(std::vector<double, std::allocator<double> > const&,
   std::vector<std::vector<std::complex<double>, std::allocator<std::complex<double> > >,
   std::allocator<std::vector<std::complex<double>, std::allocator<std::complex<double> > > > >
   const&, std::vector<double, std::allocator<double> > const&, RationalApproxMatrix const&, int) */

void __thiscall
cColumnNormEnforcer::cColumnNormEnforcer
          (cColumnNormEnforcer *this,vector *param_1,vector *param_2,vector *param_3,
          RationalApproxMatrix *param_4,int param_5)

{
  undefined8 uVar1;
  void *pvVar2;
  long *plVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  void *pvVar6;
  long lVar7;
  int iVar8;
  undefined8 *puVar9;
  long lVar10;
  ulong uVar11;
  ulong uVar12;
  size_t sVar13;
  undefined8 *puVar14;
  long *plVar15;
  long lVar16;
  undefined8 *local_50;
  
  uVar1 = *(undefined8 *)param_4;
  *(int *)(this + 4) = param_5;
  *(undefined8 *)(this + 8) = 0;
  *(int *)this = (int)uVar1;
  *(undefined8 *)(this + 0x10) = 0;
  *(undefined8 *)(this + 0x18) = 0;
  *(undefined8 *)(this + 0x20) = 0;
  *(undefined8 *)(this + 0x28) = 0;
  *(undefined8 *)(this + 0x30) = 0;
  *(undefined8 *)(this + 0x38) = 0;
  uVar12 = *(long *)(param_1 + 8) - *(long *)param_1;
  *(undefined8 *)(this + 0x40) = 0;
  *(undefined8 *)(this + 0x48) = 0;
  *(undefined8 *)(this + 0x50) = 0;
  if (uVar12 == 0) {
    uVar12 = 0;
    pvVar6 = (void *)0x0;
  }
  else {
    if (0x7ffffffffffffff8 < uVar12) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 0090d323 to 0090d327 has its CatchHandler @ 0090d332 */
      std::__throw_bad_alloc();
    }
                    /* try { // try from 0090cd79 to 0090cd7d has its CatchHandler @ 0090d332 */
    pvVar6 = operator_new(uVar12);
  }
  *(void **)(this + 0x40) = pvVar6;
  *(ulong *)(this + 0x50) = uVar12 + (long)pvVar6;
  *(void **)(this + 0x48) = pvVar6;
  pvVar2 = *(void **)param_1;
  sVar13 = (long)*(void **)(param_1 + 8) - (long)pvVar2;
  if (*(void **)(param_1 + 8) != pvVar2) {
    pvVar6 = memmove(pvVar6,pvVar2,sVar13);
  }
  *(size_t *)(this + 0x48) = (long)pvVar6 + sVar13;
  *(undefined8 *)(this + 0x58) = *(undefined8 *)(*(long *)(param_1 + 8) + -8);
  uVar12 = *(long *)(param_2 + 8) - *(long *)param_2;
  *(undefined8 *)(this + 0x60) = 0;
  *(undefined8 *)(this + 0x68) = 0;
  *(undefined8 *)(this + 0x70) = 0;
  if (uVar12 == 0) {
    local_50 = (undefined8 *)0x0;
  }
  else {
    if (0x7ffffffffffffff8 < uVar12) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 0090d32d to 0090d331 has its CatchHandler @ 0090d35a */
      std::__throw_bad_alloc();
    }
                    /* try { // try from 0090ce01 to 0090ce05 has its CatchHandler @ 0090d35a */
    local_50 = operator_new(uVar12);
  }
  *(undefined8 **)(this + 0x60) = local_50;
  *(undefined8 **)(this + 0x68) = local_50;
  *(ulong *)(this + 0x70) = uVar12 + (long)local_50;
  plVar3 = *(long **)(param_2 + 8);
  for (plVar15 = *(long **)param_2; plVar3 != plVar15; plVar15 = plVar15 + 3) {
    uVar12 = plVar15[1] - *plVar15;
    *local_50 = 0;
    local_50[1] = 0;
    local_50[2] = 0;
    if (uVar12 == 0) {
      pvVar6 = (void *)0x0;
    }
    else {
      if (0x7ffffffffffffff0 < uVar12) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 0090d31e to 0090d322 has its CatchHandler @ 0090d342 */
        std::__throw_bad_alloc();
      }
                    /* try { // try from 0090ce7c to 0090ce80 has its CatchHandler @ 0090d342 */
      pvVar6 = operator_new(uVar12);
    }
    *local_50 = pvVar6;
    local_50[1] = pvVar6;
    local_50[2] = uVar12 + (long)pvVar6;
    lVar16 = *plVar15;
    if (plVar15[1] != lVar16) {
      lVar10 = plVar15[1] - lVar16;
      lVar7 = 0;
      do {
        *(undefined8 *)((long)pvVar6 + lVar7) = *(undefined8 *)(lVar16 + lVar7);
        *(undefined8 *)((long)pvVar6 + lVar7 + 8) = *(undefined8 *)(lVar16 + 8 + lVar7);
        lVar7 = lVar7 + 0x10;
      } while (lVar10 != lVar7);
      pvVar6 = (void *)((long)pvVar6 + lVar10);
    }
    local_50[1] = pvVar6;
    local_50 = local_50 + 3;
  }
  *(undefined8 **)(this + 0x68) = local_50;
  uVar12 = *(long *)(param_3 + 8) - *(long *)param_3;
  *(undefined8 *)(this + 0x78) = 0;
  *(undefined8 *)(this + 0x80) = 0;
  *(undefined8 *)(this + 0x88) = 0;
  if (uVar12 == 0) {
    pvVar6 = (void *)0x0;
  }
  else {
    if (0x7ffffffffffffff8 < uVar12) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 0090d328 to 0090d32c has its CatchHandler @ 0090d34a */
      std::__throw_bad_alloc();
    }
                    /* try { // try from 0090cf2b to 0090cf2f has its CatchHandler @ 0090d34a */
    pvVar6 = operator_new(uVar12);
  }
  *(void **)(this + 0x78) = pvVar6;
  *(ulong *)(this + 0x88) = uVar12 + (long)pvVar6;
  *(void **)(this + 0x80) = pvVar6;
  pvVar2 = *(void **)param_3;
  sVar13 = (long)*(void **)(param_3 + 8) - (long)pvVar2;
  if (*(void **)(param_3 + 8) != pvVar2) {
    pvVar6 = memmove(pvVar6,pvVar2,sVar13);
  }
  *(undefined8 *)(this + 0x90) = 0;
  *(size_t *)(this + 0x80) = (long)pvVar6 + sVar13;
  *(undefined8 *)(this + 0x98) = 0;
  *(undefined8 *)(this + 0xa0) = 0;
  *(undefined8 *)(this + 0xa8) = 0;
  *(undefined8 *)(this + 0xb0) = 0;
  *(undefined8 *)(this + 0xb8) = 0;
  *(undefined8 *)(this + 0xc0) = 0;
  *(undefined8 *)(this + 200) = 0;
  *(undefined8 *)(this + 0xd0) = 0;
  *(undefined8 *)(this + 0xd8) = 0;
  *(undefined8 *)(this + 0xe0) = 0;
  *(undefined8 *)(this + 0xe8) = 0;
  *(undefined8 *)(this + 0xf0) = 0;
  *(undefined8 *)(this + 0xf8) = 0;
  *(undefined8 *)(this + 0x100) = 0;
  *(undefined8 *)(this + 0x108) = 0;
  *(undefined8 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x118) = 0x20;
  *(undefined8 *)(this + 0x120) = 0;
  *(undefined8 *)(this + 0x128) = 0;
  *(undefined8 *)(this + 0x130) = 0;
  *(undefined4 *)(this + 0x138) = 0x20;
  *(undefined8 *)(this + 0x140) = 0;
  *(undefined8 *)(this + 0x148) = 0;
  *(undefined8 *)(this + 0x150) = 0;
  *(undefined4 *)(this + 0x158) = 0x20;
  *(undefined8 *)(this + 0x160) = 0;
  *(undefined8 *)(this + 0x168) = 0;
  *(undefined8 *)(this + 0x170) = 0;
  this[0x178] = (cColumnNormEnforcer)0x0;
  *(undefined4 *)(this + 0x180) = 0x20;
  *(undefined8 *)(this + 0x188) = 0;
  *(undefined8 *)(this + 400) = 0;
  *(undefined8 *)(this + 0x198) = 0;
  *(undefined8 *)(this + 0x1a0) = 0;
  *(undefined4 *)(this + 0x1a8) = 0;
  this[0x1ac] = (cColumnNormEnforcer)0x0;
  *(undefined8 *)(this + 0x1b0) = 0;
  *(undefined8 *)(this + 0x1b8) = 0;
  *(undefined8 *)(this + 0x1c0) = 0;
  *(undefined8 *)(this + 0x1c8) = 0;
  *(undefined8 *)(this + 0x1d0) = 0;
  *(undefined8 *)(this + 0x1d8) = 0;
  *(undefined8 *)(this + 0x1e0) = 0;
  *(undefined8 *)(this + 0x1e8) = 0;
  *(undefined8 *)(this + 0x1f0) = 0;
                    /* try { // try from 0090d173 to 0090d177 has its CatchHandler @ 0090d33a */
  cBlockQR::cBlockQR((cBlockQR *)(this + 0x1f8));
  uVar1 = DAT_00ab67a8;
  puVar4 = *(undefined8 **)(this + 0x18);
  this[0x270] = (cColumnNormEnforcer)0x1;
  uVar5 = _DAT_00ab67f8;
  *(undefined8 *)(this + 0x278) = uVar1;
  iVar8 = *(int *)this;
  uVar11 = (ulong)iVar8;
  *(undefined8 *)(this + 0x280) = uVar5;
  uVar12 = (long)puVar4 - *(long *)(this + 0x10) >> 6;
  *(undefined8 *)(this + 0x288) = uVar5;
  *(undefined8 *)(this + 0x290) = uVar5;
  if (uVar12 < uVar11) {
    std::vector<PoleResidueForm,std::allocator<PoleResidueForm>>::_M_default_append
              ((vector<PoleResidueForm,std::allocator<PoleResidueForm>> *)(this + 0x10),
               uVar11 - uVar12);
    iVar8 = *(int *)this;
  }
  else if ((uVar11 < uVar12) &&
          (puVar9 = (undefined8 *)(*(long *)(this + 0x10) + uVar11 * 0x40), puVar14 = puVar9,
          puVar4 != puVar9)) {
    do {
      if ((void *)puVar14[3] != (void *)0x0) {
        tc_cfree((void *)puVar14[3]);
      }
      if ((void *)*puVar14 != (void *)0x0) {
        tc_cfree((void *)*puVar14);
      }
      puVar14 = puVar14 + 8;
    } while (puVar4 != puVar14);
    *(undefined8 **)(this + 0x18) = puVar9;
    iVar8 = *(int *)this;
  }
  lVar16 = 0;
  if (0 < iVar8) {
    do {
      lVar10 = lVar16 * 0x40 + *(long *)(this + 0x10);
      lVar7 = ((long)param_5 * *(long *)param_4 + lVar16) * 0x40 + *(long *)(param_4 + 0x10);
                    /* try { // try from 0090d20d to 0090d30c has its CatchHandler @ 0090d352 */
      FUN_0090b840(lVar10,lVar7);
      FUN_0090b840(lVar10 + 0x18,lVar7 + 0x18);
      lVar16 = lVar16 + 1;
      *(undefined8 *)(lVar10 + 0x30) = *(undefined8 *)(lVar7 + 0x30);
      *(undefined8 *)(lVar10 + 0x38) = *(undefined8 *)(lVar7 + 0x38);
    } while ((int)lVar16 < *(int *)this);
  }
  this[0x1ac] = (cColumnNormEnforcer)
                (**(double **)param_1 <= DAT_00ab7270 && DAT_00ab7270 != **(double **)param_1);
  NormalizeFrequencies(this);
  SetPoleRanges(this);
  return;
}


