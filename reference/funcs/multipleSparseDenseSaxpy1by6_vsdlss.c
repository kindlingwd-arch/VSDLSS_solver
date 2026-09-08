/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSparseDenseSaxpy1by6_vsdlss  addr: 00a1d040  size: 34e */
#include "vsdlss_ref.h"

/* signature: void multipleSparseDenseSaxpy1by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6); */

void multipleSparseDenseSaxpy1by6_vsdlss
               (undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int *piVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  
  lVar15 = *(long *)(param_6 + 8);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x28b,6,"multipleSparseDenseSaxpy1by6");
    return;
  case 1:
    dVar9 = *(double *)(param_3 + 0x40);
    lVar16 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      lVar18 = 0;
      do {
        piVar8 = (int *)(param_5 + 4 + lVar18);
        lVar19 = lVar18 * 2;
        lVar18 = lVar18 + 4;
        pdVar1 = (double *)(lVar15 + (long)*piVar8 * 8);
        *pdVar1 = *(double *)(lVar16 + 8 + lVar19) * dVar9 + *pdVar1;
      } while (lVar18 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 2:
    lVar16 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x10);
    dVar9 = *(double *)(param_3 + 0x40);
    dVar10 = *(double *)(param_3 + 0x48);
    if (0 < param_2) {
      lVar19 = 0;
      do {
        lVar20 = lVar19 * 2;
        piVar8 = (int *)(param_5 + 4 + lVar19);
        lVar21 = lVar19 * 2;
        lVar19 = lVar19 + 4;
        pdVar1 = (double *)(lVar15 + (long)*piVar8 * 8);
        *pdVar1 = *(double *)(lVar16 + 8 + lVar20) * dVar9 +
                  *(double *)(lVar18 + 8 + lVar21) * dVar10 + *pdVar1;
      } while (lVar19 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 3:
    lVar16 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x10);
    dVar9 = *(double *)(param_3 + 0x40);
    lVar19 = *(long *)(param_4 + 0x18);
    dVar10 = *(double *)(param_3 + 0x48);
    dVar11 = *(double *)(param_3 + 0x50);
    if (0 < param_2) {
      lVar20 = 0;
      do {
        lVar21 = lVar20 * 2;
        lVar22 = lVar20 * 2;
        pdVar1 = (double *)(lVar15 + (long)*(int *)(param_5 + 4 + lVar20) * 8);
        lVar17 = lVar20 * 2;
        lVar20 = lVar20 + 4;
        *pdVar1 = *(double *)(lVar16 + 8 + lVar21) * dVar9 +
                  *(double *)(lVar18 + 8 + lVar22) * dVar10 +
                  *(double *)(lVar19 + 8 + lVar17) * dVar11 + *pdVar1;
      } while (lVar20 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 4:
    lVar16 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x10);
    lVar19 = *(long *)(param_4 + 0x18);
    dVar9 = *(double *)(param_3 + 0x40);
    dVar10 = *(double *)(param_3 + 0x48);
    lVar20 = *(long *)(param_4 + 0x20);
    dVar11 = *(double *)(param_3 + 0x50);
    dVar12 = *(double *)(param_3 + 0x58);
    if (0 < param_2) {
      lVar21 = 0;
      do {
        lVar22 = lVar21 * 2;
        lVar17 = lVar21 * 2;
        pdVar1 = (double *)(lVar15 + (long)*(int *)(param_5 + 4 + lVar21) * 8);
        lVar2 = lVar21 * 2;
        lVar3 = lVar21 * 2;
        lVar21 = lVar21 + 4;
        *pdVar1 = *(double *)(lVar16 + 8 + lVar22) * dVar9 +
                  *(double *)(lVar18 + 8 + lVar17) * dVar10 +
                  *(double *)(lVar19 + 8 + lVar2) * dVar11 +
                  *(double *)(lVar20 + 8 + lVar3) * dVar12 + *pdVar1;
      } while (lVar21 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 5:
    lVar16 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x10);
    lVar19 = *(long *)(param_4 + 0x18);
    lVar20 = *(long *)(param_4 + 0x20);
    dVar9 = *(double *)(param_3 + 0x40);
    lVar21 = *(long *)(param_4 + 0x28);
    dVar10 = *(double *)(param_3 + 0x48);
    dVar11 = *(double *)(param_3 + 0x50);
    dVar12 = *(double *)(param_3 + 0x58);
    dVar13 = *(double *)(param_3 + 0x60);
    if (0 < param_2) {
      lVar22 = 0;
      do {
        lVar17 = lVar22 * 2;
        lVar2 = lVar22 * 2;
        pdVar1 = (double *)(lVar15 + (long)*(int *)(param_5 + 4 + lVar22) * 8);
        lVar3 = lVar22 * 2;
        lVar4 = lVar22 * 2;
        lVar5 = lVar22 * 2;
        lVar22 = lVar22 + 4;
        *pdVar1 = *(double *)(lVar16 + 8 + lVar17) * dVar9 +
                  *(double *)(lVar18 + 8 + lVar2) * dVar10 +
                  *(double *)(lVar19 + 8 + lVar3) * dVar11 +
                  *(double *)(lVar20 + 8 + lVar4) * dVar12 +
                  *(double *)(lVar21 + 8 + lVar5) * dVar13 + *pdVar1;
      } while (lVar22 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 6:
    lVar16 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x10);
    lVar19 = *(long *)(param_4 + 0x18);
    lVar20 = *(long *)(param_4 + 0x20);
    lVar21 = *(long *)(param_4 + 0x28);
    dVar9 = *(double *)(param_3 + 0x40);
    dVar10 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 0x30);
    dVar11 = *(double *)(param_3 + 0x50);
    dVar12 = *(double *)(param_3 + 0x58);
    dVar13 = *(double *)(param_3 + 0x60);
    dVar14 = *(double *)(param_3 + 0x68);
    if (0 < param_2) {
      lVar17 = 0;
      do {
        lVar2 = lVar17 * 2;
        lVar3 = lVar17 * 2;
        pdVar1 = (double *)(lVar15 + (long)*(int *)(param_5 + 4 + lVar17) * 8);
        lVar4 = lVar17 * 2;
        lVar5 = lVar17 * 2;
        lVar6 = lVar17 * 2;
        lVar7 = lVar17 * 2;
        lVar17 = lVar17 + 4;
        *pdVar1 = *(double *)(lVar16 + 8 + lVar2) * dVar9 + *(double *)(lVar18 + 8 + lVar3) * dVar10
                  + *(double *)(lVar19 + 8 + lVar4) * dVar11 +
                  *(double *)(lVar20 + 8 + lVar5) * dVar12 +
                  *(double *)(lVar21 + 8 + lVar6) * dVar13 +
                  *(double *)(lVar22 + 8 + lVar7) * dVar14 + *pdVar1;
      } while (lVar17 != (ulong)(param_2 - 1) * 4 + 4);
    }
  }
  return;
}


