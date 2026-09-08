/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSparseDenseSaxpy3by6_vsdlss  addr: 00a1d970  size: 82b */
#include "vsdlss_ref.h"

/* signature: void multipleSparseDenseSaxpy3by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6); */

void multipleSparseDenseSaxpy3by6_vsdlss
               (undefined4 param_1,int param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  double dVar4;
  double dVar5;
  double dVar6;
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
  double dVar19;
  double dVar20;
  double dVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long lVar28;
  long lVar29;
  long lVar30;
  long lVar31;
  long lVar32;
  long lVar33;
  long lVar34;
  long lVar35;
  
  lVar22 = *(long *)(param_6 + 8);
  lVar23 = *(long *)(param_6 + 0x10);
  lVar24 = *(long *)(param_6 + 0x18);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x40a,6,"multipleSparseDenseSaxpy3by6");
    return;
  case 1:
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x78);
    lVar25 = *(long *)(param_4 + 8);
    dVar6 = *(double *)(param_3 + 0xb0);
    if (0 < param_2) {
      lVar27 = 0;
      do {
        lVar29 = (long)*(int *)(param_5 + 4 + lVar27) * 8;
        *(double *)(lVar22 + lVar29) =
             *(double *)(lVar25 + 8 + lVar27 * 2) * dVar4 + *(double *)(lVar22 + lVar29);
        *(double *)(lVar23 + lVar29) =
             *(double *)(lVar25 + 8 + lVar27 * 2) * dVar5 + *(double *)(lVar23 + lVar29);
        lVar28 = lVar27 * 2;
        lVar27 = lVar27 + 4;
        *(double *)(lVar29 + lVar24) =
             *(double *)(lVar25 + 8 + lVar28) * dVar6 + *(double *)(lVar29 + lVar24);
      } while (lVar27 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 2:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    dVar6 = *(double *)(param_3 + 0x78);
    dVar7 = *(double *)(param_3 + 0x80);
    dVar8 = *(double *)(param_3 + 0xb0);
    dVar9 = *(double *)(param_3 + 0xb8);
    if (0 < param_2) {
      lVar28 = 0;
      do {
        lVar31 = (long)*(int *)(param_5 + 4 + lVar28) * 8;
        *(double *)(lVar22 + lVar31) =
             *(double *)(lVar25 + 8 + lVar28 * 2) * dVar4 +
             *(double *)(lVar27 + 8 + lVar28 * 2) * dVar5 + *(double *)(lVar22 + lVar31);
        *(double *)(lVar23 + lVar31) =
             *(double *)(lVar25 + 8 + lVar28 * 2) * dVar6 +
             *(double *)(lVar27 + 8 + lVar28 * 2) * dVar7 + *(double *)(lVar23 + lVar31);
        lVar29 = lVar28 * 2;
        lVar30 = lVar28 * 2;
        lVar28 = lVar28 + 4;
        *(double *)(lVar31 + lVar24) =
             *(double *)(lVar25 + 8 + lVar29) * dVar8 + *(double *)(lVar27 + 8 + lVar30) * dVar9 +
             *(double *)(lVar31 + lVar24);
      } while (lVar28 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 3:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar4 = *(double *)(param_3 + 0x40);
    lVar28 = *(long *)(param_4 + 0x18);
    dVar5 = *(double *)(param_3 + 0x48);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x78);
    dVar8 = *(double *)(param_3 + 0x80);
    dVar9 = *(double *)(param_3 + 0x88);
    dVar10 = *(double *)(param_3 + 0xb0);
    dVar11 = *(double *)(param_3 + 0xb8);
    dVar12 = *(double *)(param_3 + 0xc0);
    if (0 < param_2) {
      lVar29 = 0;
      do {
        lVar33 = (long)*(int *)(param_5 + 4 + lVar29) * 8;
        *(double *)(lVar22 + lVar33) =
             *(double *)(lVar25 + 8 + lVar29 * 2) * dVar4 +
             *(double *)(lVar27 + 8 + lVar29 * 2) * dVar5 +
             *(double *)(lVar28 + 8 + lVar29 * 2) * dVar6 + *(double *)(lVar22 + lVar33);
        *(double *)(lVar23 + lVar33) =
             *(double *)(lVar25 + 8 + lVar29 * 2) * dVar7 +
             *(double *)(lVar27 + 8 + lVar29 * 2) * dVar8 +
             *(double *)(lVar28 + 8 + lVar29 * 2) * dVar9 + *(double *)(lVar23 + lVar33);
        lVar30 = lVar29 * 2;
        lVar31 = lVar29 * 2;
        lVar26 = lVar29 * 2;
        lVar29 = lVar29 + 4;
        *(double *)(lVar33 + lVar24) =
             *(double *)(lVar25 + 8 + lVar30) * dVar10 + *(double *)(lVar27 + 8 + lVar31) * dVar11 +
             *(double *)(lVar28 + 8 + lVar26) * dVar12 + *(double *)(lVar33 + lVar24);
      } while (lVar29 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 4:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    lVar28 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar29 = *(long *)(param_4 + 0x20);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x58);
    dVar8 = *(double *)(param_3 + 0x78);
    dVar9 = *(double *)(param_3 + 0x80);
    dVar10 = *(double *)(param_3 + 0x88);
    dVar11 = *(double *)(param_3 + 0x90);
    dVar12 = *(double *)(param_3 + 0xb0);
    dVar13 = *(double *)(param_3 + 0xb8);
    dVar14 = *(double *)(param_3 + 0xc0);
    dVar15 = *(double *)(param_3 + 200);
    if (0 < param_2) {
      lVar30 = 0;
      do {
        lVar34 = (long)*(int *)(param_5 + 4 + lVar30) * 8;
        *(double *)(lVar22 + lVar34) =
             *(double *)(lVar25 + 8 + lVar30 * 2) * dVar4 +
             *(double *)(lVar27 + 8 + lVar30 * 2) * dVar5 +
             *(double *)(lVar28 + 8 + lVar30 * 2) * dVar6 +
             *(double *)(lVar29 + 8 + lVar30 * 2) * dVar7 + *(double *)(lVar22 + lVar34);
        *(double *)(lVar23 + lVar34) =
             *(double *)(lVar25 + 8 + lVar30 * 2) * dVar8 +
             *(double *)(lVar27 + 8 + lVar30 * 2) * dVar9 +
             *(double *)(lVar28 + 8 + lVar30 * 2) * dVar10 +
             *(double *)(lVar29 + 8 + lVar30 * 2) * dVar11 + *(double *)(lVar23 + lVar34);
        lVar31 = lVar30 * 2;
        lVar26 = lVar30 * 2;
        lVar33 = lVar30 * 2;
        lVar1 = lVar30 * 2;
        lVar30 = lVar30 + 4;
        *(double *)(lVar34 + lVar24) =
             *(double *)(lVar25 + 8 + lVar31) * dVar12 + *(double *)(lVar27 + 8 + lVar26) * dVar13 +
             *(double *)(lVar28 + 8 + lVar33) * dVar14 + *(double *)(lVar29 + 8 + lVar1) * dVar15 +
             *(double *)(lVar34 + lVar24);
      } while (lVar30 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 5:
    dVar4 = *(double *)(param_3 + 0x40);
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    lVar28 = *(long *)(param_4 + 0x18);
    lVar29 = *(long *)(param_4 + 0x20);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar30 = *(long *)(param_4 + 0x28);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x58);
    dVar8 = *(double *)(param_3 + 0x60);
    dVar9 = *(double *)(param_3 + 0x78);
    dVar10 = *(double *)(param_3 + 0x80);
    dVar11 = *(double *)(param_3 + 0x88);
    dVar12 = *(double *)(param_3 + 0x90);
    dVar13 = *(double *)(param_3 + 0x98);
    dVar14 = *(double *)(param_3 + 0xb0);
    dVar15 = *(double *)(param_3 + 0xb8);
    dVar16 = *(double *)(param_3 + 0xc0);
    dVar17 = *(double *)(param_3 + 200);
    dVar18 = *(double *)(param_3 + 0xd0);
    if (0 < param_2) {
      lVar31 = 0;
      do {
        lVar35 = (long)*(int *)(param_5 + 4 + lVar31) * 8;
        *(double *)(lVar22 + lVar35) =
             dVar4 * *(double *)(lVar25 + 8 + lVar31 * 2) +
             *(double *)(lVar27 + 8 + lVar31 * 2) * dVar5 +
             *(double *)(lVar28 + 8 + lVar31 * 2) * dVar6 +
             *(double *)(lVar29 + 8 + lVar31 * 2) * dVar7 +
             *(double *)(lVar30 + 8 + lVar31 * 2) * dVar8 + *(double *)(lVar22 + lVar35);
        *(double *)(lVar23 + lVar35) =
             *(double *)(lVar25 + 8 + lVar31 * 2) * dVar9 +
             *(double *)(lVar27 + 8 + lVar31 * 2) * dVar10 +
             *(double *)(lVar28 + 8 + lVar31 * 2) * dVar11 +
             *(double *)(lVar29 + 8 + lVar31 * 2) * dVar12 +
             *(double *)(lVar30 + 8 + lVar31 * 2) * dVar13 + *(double *)(lVar23 + lVar35);
        lVar26 = lVar31 * 2;
        lVar33 = lVar31 * 2;
        lVar1 = lVar31 * 2;
        lVar34 = lVar31 * 2;
        lVar2 = lVar31 * 2;
        lVar31 = lVar31 + 4;
        *(double *)(lVar35 + lVar24) =
             *(double *)(lVar25 + 8 + lVar26) * dVar14 + *(double *)(lVar27 + 8 + lVar33) * dVar15 +
             *(double *)(lVar28 + 8 + lVar1) * dVar16 + *(double *)(lVar29 + 8 + lVar34) * dVar17 +
             *(double *)(lVar30 + 8 + lVar2) * dVar18 + *(double *)(lVar35 + lVar24);
      } while (lVar31 != (ulong)(param_2 - 1) * 4 + 4);
      return;
    }
    break;
  case 6:
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar25 = *(long *)(param_4 + 8);
    dVar6 = *(double *)(param_3 + 0x50);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar7 = *(double *)(param_3 + 0x58);
    lVar28 = *(long *)(param_4 + 0x18);
    lVar29 = *(long *)(param_4 + 0x20);
    lVar30 = *(long *)(param_4 + 0x28);
    lVar31 = *(long *)(param_4 + 0x30);
    dVar8 = *(double *)(param_3 + 0x60);
    dVar9 = *(double *)(param_3 + 0x68);
    dVar10 = *(double *)(param_3 + 0x78);
    dVar11 = *(double *)(param_3 + 0x80);
    dVar12 = *(double *)(param_3 + 0x88);
    dVar13 = *(double *)(param_3 + 0x90);
    dVar14 = *(double *)(param_3 + 0x98);
    dVar15 = *(double *)(param_3 + 0xa0);
    dVar16 = *(double *)(param_3 + 0xb0);
    dVar17 = *(double *)(param_3 + 0xb8);
    dVar18 = *(double *)(param_3 + 0xc0);
    dVar19 = *(double *)(param_3 + 200);
    dVar20 = *(double *)(param_3 + 0xd0);
    dVar21 = *(double *)(param_3 + 0xd8);
    if (0 < param_2) {
      lVar26 = 0;
      do {
        lVar32 = (long)*(int *)(param_5 + 4 + lVar26) * 8;
        *(double *)(lVar22 + lVar32) =
             dVar4 * *(double *)(lVar25 + 8 + lVar26 * 2) +
             dVar5 * *(double *)(lVar27 + 8 + lVar26 * 2) +
             dVar6 * *(double *)(lVar28 + 8 + lVar26 * 2) +
             dVar7 * *(double *)(lVar29 + 8 + lVar26 * 2) +
             *(double *)(lVar30 + 8 + lVar26 * 2) * dVar8 +
             *(double *)(lVar31 + 8 + lVar26 * 2) * dVar9 + *(double *)(lVar22 + lVar32);
        *(double *)(lVar23 + lVar32) =
             *(double *)(lVar25 + 8 + lVar26 * 2) * dVar10 +
             *(double *)(lVar27 + 8 + lVar26 * 2) * dVar11 +
             *(double *)(lVar28 + 8 + lVar26 * 2) * dVar12 +
             *(double *)(lVar29 + 8 + lVar26 * 2) * dVar13 +
             *(double *)(lVar30 + 8 + lVar26 * 2) * dVar14 +
             *(double *)(lVar31 + 8 + lVar26 * 2) * dVar15 + *(double *)(lVar23 + lVar32);
        lVar33 = lVar26 * 2;
        lVar1 = lVar26 * 2;
        lVar34 = lVar26 * 2;
        lVar2 = lVar26 * 2;
        lVar35 = lVar26 * 2;
        lVar3 = lVar26 * 2;
        lVar26 = lVar26 + 4;
        *(double *)(lVar32 + lVar24) =
             *(double *)(lVar25 + 8 + lVar33) * dVar16 + *(double *)(lVar27 + 8 + lVar1) * dVar17 +
             *(double *)(lVar28 + 8 + lVar34) * dVar18 + *(double *)(lVar29 + 8 + lVar2) * dVar19 +
             *(double *)(lVar30 + 8 + lVar35) * dVar20 + *(double *)(lVar31 + 8 + lVar3) * dVar21 +
             *(double *)(lVar32 + lVar24);
      } while (lVar26 != (ulong)(param_2 - 1) * 4 + 4);
    }
  }
  return;
}


