/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSparseDenseSaxpy2by6_vsdlss  addr: 00a1d3c0  size: 59b */
#include "vsdlss_ref.h"

/* signature: void multipleSparseDenseSaxpy2by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6); */

void multipleSparseDenseSaxpy2by6_vsdlss
               (undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long lVar28;
  double *pdVar29;
  
  lVar19 = *(long *)(param_6 + 8);
  lVar20 = *(long *)(param_6 + 0x10);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x33a,6,"multipleSparseDenseSaxpy2by6");
    return;
  case 1:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x78);
    lVar21 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      lVar23 = 0;
      do {
        lVar24 = (long)*(int *)(param_5 + 4 + lVar23) * 8;
        pdVar1 = (double *)(lVar19 + lVar24);
        pdVar29 = (double *)(lVar24 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar23 * 2) * dVar7 + *pdVar1;
        lVar24 = lVar23 * 2;
        lVar23 = lVar23 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar24) * dVar8 + *pdVar29;
      } while (lVar23 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 2:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 8);
    dVar9 = *(double *)(param_3 + 0x78);
    lVar23 = *(long *)(param_4 + 0x10);
    dVar10 = *(double *)(param_3 + 0x80);
    if (0 < param_2) {
      lVar24 = 0;
      do {
        lVar25 = (long)*(int *)(param_5 + 4 + lVar24) * 8;
        pdVar1 = (double *)(lVar19 + lVar25);
        pdVar29 = (double *)(lVar25 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar24 * 2) * dVar7 +
                  *(double *)(lVar23 + 8 + lVar24 * 2) * dVar8 + *pdVar1;
        lVar25 = lVar24 * 2;
        lVar26 = lVar24 * 2;
        lVar24 = lVar24 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar25) * dVar9 +
                   *(double *)(lVar23 + 8 + lVar26) * dVar10 + *pdVar29;
      } while (lVar24 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 3:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 8);
    dVar9 = *(double *)(param_3 + 0x50);
    lVar23 = *(long *)(param_4 + 0x10);
    dVar10 = *(double *)(param_3 + 0x78);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar11 = *(double *)(param_3 + 0x80);
    dVar12 = *(double *)(param_3 + 0x88);
    if (0 < param_2) {
      lVar25 = 0;
      do {
        lVar26 = (long)*(int *)(param_5 + 4 + lVar25) * 8;
        pdVar1 = (double *)(lVar19 + lVar26);
        pdVar29 = (double *)(lVar26 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar25 * 2) * dVar7 +
                  *(double *)(lVar23 + 8 + lVar25 * 2) * dVar8 +
                  *(double *)(lVar24 + 8 + lVar25 * 2) * dVar9 + *pdVar1;
        lVar26 = lVar25 * 2;
        lVar27 = lVar25 * 2;
        lVar22 = lVar25 * 2;
        lVar25 = lVar25 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar26) * dVar10 +
                   *(double *)(lVar23 + 8 + lVar27) * dVar11 +
                   *(double *)(lVar24 + 8 + lVar22) * dVar12 + *pdVar29;
      } while (lVar25 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 4:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 8);
    dVar9 = *(double *)(param_3 + 0x50);
    lVar23 = *(long *)(param_4 + 0x10);
    dVar10 = *(double *)(param_3 + 0x58);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar11 = *(double *)(param_3 + 0x78);
    lVar25 = *(long *)(param_4 + 0x20);
    dVar12 = *(double *)(param_3 + 0x80);
    dVar13 = *(double *)(param_3 + 0x88);
    dVar14 = *(double *)(param_3 + 0x90);
    if (0 < param_2) {
      lVar26 = 0;
      do {
        lVar27 = (long)*(int *)(param_5 + 4 + lVar26) * 8;
        pdVar1 = (double *)(lVar19 + lVar27);
        pdVar29 = (double *)(lVar27 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar26 * 2) * dVar7 +
                  *(double *)(lVar23 + 8 + lVar26 * 2) * dVar8 +
                  *(double *)(lVar24 + 8 + lVar26 * 2) * dVar9 +
                  *(double *)(lVar25 + 8 + lVar26 * 2) * dVar10 + *pdVar1;
        lVar27 = lVar26 * 2;
        lVar22 = lVar26 * 2;
        lVar28 = lVar26 * 2;
        lVar2 = lVar26 * 2;
        lVar26 = lVar26 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar27) * dVar11 +
                   *(double *)(lVar23 + 8 + lVar22) * dVar12 +
                   *(double *)(lVar24 + 8 + lVar28) * dVar13 +
                   *(double *)(lVar25 + 8 + lVar2) * dVar14 + *pdVar29;
      } while (lVar26 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 5:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 8);
    dVar9 = *(double *)(param_3 + 0x50);
    lVar23 = *(long *)(param_4 + 0x10);
    dVar10 = *(double *)(param_3 + 0x58);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar11 = *(double *)(param_3 + 0x60);
    lVar25 = *(long *)(param_4 + 0x20);
    dVar12 = *(double *)(param_3 + 0x78);
    lVar26 = *(long *)(param_4 + 0x28);
    dVar13 = *(double *)(param_3 + 0x80);
    dVar14 = *(double *)(param_3 + 0x88);
    dVar15 = *(double *)(param_3 + 0x90);
    dVar16 = *(double *)(param_3 + 0x98);
    if (0 < param_2) {
      lVar27 = 0;
      do {
        lVar22 = (long)*(int *)(param_5 + 4 + lVar27) * 8;
        pdVar1 = (double *)(lVar19 + lVar22);
        pdVar29 = (double *)(lVar22 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar27 * 2) * dVar7 +
                  *(double *)(lVar23 + 8 + lVar27 * 2) * dVar8 +
                  *(double *)(lVar24 + 8 + lVar27 * 2) * dVar9 +
                  *(double *)(lVar25 + 8 + lVar27 * 2) * dVar10 +
                  *(double *)(lVar26 + 8 + lVar27 * 2) * dVar11 + *pdVar1;
        lVar22 = lVar27 * 2;
        lVar28 = lVar27 * 2;
        lVar2 = lVar27 * 2;
        lVar3 = lVar27 * 2;
        lVar4 = lVar27 * 2;
        lVar27 = lVar27 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar22) * dVar12 +
                   *(double *)(lVar23 + 8 + lVar28) * dVar13 +
                   *(double *)(lVar24 + 8 + lVar2) * dVar14 +
                   *(double *)(lVar25 + 8 + lVar3) * dVar15 +
                   *(double *)(lVar26 + 8 + lVar4) * dVar16 + *pdVar29;
      } while (lVar27 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 6:
    dVar7 = *(double *)(param_3 + 0x40);
    dVar8 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 8);
    dVar9 = *(double *)(param_3 + 0x50);
    lVar23 = *(long *)(param_4 + 0x10);
    dVar10 = *(double *)(param_3 + 0x58);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar11 = *(double *)(param_3 + 0x60);
    lVar25 = *(long *)(param_4 + 0x20);
    dVar12 = *(double *)(param_3 + 0x68);
    lVar26 = *(long *)(param_4 + 0x28);
    dVar13 = *(double *)(param_3 + 0x78);
    lVar27 = *(long *)(param_4 + 0x30);
    dVar14 = *(double *)(param_3 + 0x80);
    dVar15 = *(double *)(param_3 + 0x88);
    dVar16 = *(double *)(param_3 + 0x90);
    dVar17 = *(double *)(param_3 + 0x98);
    dVar18 = *(double *)(param_3 + 0xa0);
    if (0 < param_2) {
      lVar22 = 0;
      do {
        lVar28 = (long)*(int *)(param_5 + 4 + lVar22) * 8;
        pdVar1 = (double *)(lVar19 + lVar28);
        pdVar29 = (double *)(lVar28 + lVar20);
        *pdVar1 = *(double *)(lVar21 + 8 + lVar22 * 2) * dVar7 +
                  *(double *)(lVar23 + 8 + lVar22 * 2) * dVar8 +
                  *(double *)(lVar24 + 8 + lVar22 * 2) * dVar9 +
                  *(double *)(lVar25 + 8 + lVar22 * 2) * dVar10 +
                  *(double *)(lVar26 + 8 + lVar22 * 2) * dVar11 +
                  *(double *)(lVar27 + 8 + lVar22 * 2) * dVar12 + *pdVar1;
        lVar28 = lVar22 * 2;
        lVar2 = lVar22 * 2;
        lVar3 = lVar22 * 2;
        lVar4 = lVar22 * 2;
        lVar5 = lVar22 * 2;
        lVar6 = lVar22 * 2;
        lVar22 = lVar22 + 4;
        *pdVar29 = *(double *)(lVar21 + 8 + lVar28) * dVar13 +
                   *(double *)(lVar23 + 8 + lVar2) * dVar14 +
                   *(double *)(lVar24 + 8 + lVar3) * dVar15 +
                   *(double *)(lVar25 + 8 + lVar4) * dVar16 +
                   *(double *)(lVar26 + 8 + lVar5) * dVar17 +
                   *(double *)(lVar27 + 8 + lVar6) * dVar18 + *pdVar29;
      } while (lVar22 != (ulong)(param_2 - 1) * 4 + 4);
    }
  }
  return;
}


