/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSaxpy3by6_vsdlss  addr: 00a1c830  size: 7f2 */
#include "vsdlss_ref.h"

/* signature: void multipleSaxpy3by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5); */

void multipleSaxpy3by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5)

{
  double dVar1;
  double dVar2;
  double dVar3;
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
  
  lVar19 = *(long *)(param_5 + 8);
  lVar20 = *(long *)(param_5 + 0x10);
  lVar21 = *(long *)(param_5 + 0x18);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x1fd,6,"multipleSaxpy3by6");
    return;
  case 1:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x78);
    dVar3 = *(double *)(param_3 + 0xb0);
    lVar22 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      lVar24 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar24) =
             *(double *)(lVar22 + 8 + lVar24) * dVar1 + *(double *)(lVar19 + 8 + lVar24);
        *(double *)(lVar20 + 8 + lVar24) =
             *(double *)(lVar22 + 8 + lVar24) * dVar2 + *(double *)(lVar20 + 8 + lVar24);
        *(double *)(lVar21 + 8 + lVar24) =
             *(double *)(lVar22 + 8 + lVar24) * dVar3 + *(double *)(lVar21 + 8 + lVar24);
        lVar24 = lVar24 + 8;
      } while (lVar24 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 2:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x78);
    dVar4 = *(double *)(param_3 + 0x80);
    dVar5 = *(double *)(param_3 + 0xb0);
    dVar6 = *(double *)(param_3 + 0xb8);
    lVar22 = *(long *)(param_4 + 8);
    lVar24 = *(long *)(param_4 + 0x10);
    if (0 < param_2) {
      lVar25 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar25) =
             *(double *)(lVar22 + 8 + lVar25) * dVar1 + *(double *)(lVar24 + 8 + lVar25) * dVar2 +
             *(double *)(lVar19 + 8 + lVar25);
        *(double *)(lVar20 + 8 + lVar25) =
             *(double *)(lVar22 + 8 + lVar25) * dVar3 + *(double *)(lVar24 + 8 + lVar25) * dVar4 +
             *(double *)(lVar20 + 8 + lVar25);
        *(double *)(lVar21 + 8 + lVar25) =
             *(double *)(lVar22 + 8 + lVar25) * dVar5 + *(double *)(lVar24 + 8 + lVar25) * dVar6 +
             *(double *)(lVar21 + 8 + lVar25);
        lVar25 = lVar25 + 8;
      } while (lVar25 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 3:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x78);
    dVar5 = *(double *)(param_3 + 0x80);
    dVar6 = *(double *)(param_3 + 0x88);
    dVar7 = *(double *)(param_3 + 0xb0);
    dVar8 = *(double *)(param_3 + 0xb8);
    dVar9 = *(double *)(param_3 + 0xc0);
    lVar24 = *(long *)(param_4 + 8);
    lVar25 = *(long *)(param_4 + 0x18);
    if (0 < param_2) {
      lVar26 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar26) =
             *(double *)(lVar24 + 8 + lVar26) * dVar1 + *(double *)(lVar22 + 8 + lVar26) * dVar2 +
             *(double *)(lVar25 + 8 + lVar26) * dVar3 + *(double *)(lVar19 + 8 + lVar26);
        *(double *)(lVar20 + 8 + lVar26) =
             *(double *)(lVar24 + 8 + lVar26) * dVar4 + *(double *)(lVar22 + 8 + lVar26) * dVar5 +
             *(double *)(lVar25 + 8 + lVar26) * dVar6 + *(double *)(lVar20 + 8 + lVar26);
        *(double *)(lVar21 + 8 + lVar26) =
             *(double *)(lVar24 + 8 + lVar26) * dVar7 + *(double *)(lVar22 + 8 + lVar26) * dVar8 +
             *(double *)(lVar25 + 8 + lVar26) * dVar9 + *(double *)(lVar21 + 8 + lVar26);
        lVar26 = lVar26 + 8;
      } while (lVar26 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 4:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x78);
    dVar6 = *(double *)(param_3 + 0x80);
    dVar7 = *(double *)(param_3 + 0x88);
    dVar8 = *(double *)(param_3 + 0x90);
    dVar9 = *(double *)(param_3 + 0xb0);
    dVar10 = *(double *)(param_3 + 0xb8);
    dVar11 = *(double *)(param_3 + 0xc0);
    dVar12 = *(double *)(param_3 + 200);
    lVar25 = *(long *)(param_4 + 8);
    lVar26 = *(long *)(param_4 + 0x20);
    if (0 < param_2) {
      lVar27 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar27) =
             *(double *)(lVar25 + 8 + lVar27) * dVar1 + *(double *)(lVar22 + 8 + lVar27) * dVar2 +
             *(double *)(lVar24 + 8 + lVar27) * dVar3 + *(double *)(lVar26 + 8 + lVar27) * dVar4 +
             *(double *)(lVar19 + 8 + lVar27);
        *(double *)(lVar20 + 8 + lVar27) =
             *(double *)(lVar25 + 8 + lVar27) * dVar5 + *(double *)(lVar22 + 8 + lVar27) * dVar6 +
             *(double *)(lVar24 + 8 + lVar27) * dVar7 + *(double *)(lVar26 + 8 + lVar27) * dVar8 +
             *(double *)(lVar20 + 8 + lVar27);
        *(double *)(lVar21 + 8 + lVar27) =
             *(double *)(lVar25 + 8 + lVar27) * dVar9 + *(double *)(lVar22 + 8 + lVar27) * dVar10 +
             *(double *)(lVar24 + 8 + lVar27) * dVar11 + *(double *)(lVar26 + 8 + lVar27) * dVar12 +
             *(double *)(lVar21 + 8 + lVar27);
        lVar27 = lVar27 + 8;
      } while (lVar27 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 5:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 0x10);
    lVar24 = *(long *)(param_4 + 0x18);
    lVar25 = *(long *)(param_4 + 0x20);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x78);
    dVar7 = *(double *)(param_3 + 0x80);
    dVar8 = *(double *)(param_3 + 0x88);
    dVar9 = *(double *)(param_3 + 0x90);
    dVar10 = *(double *)(param_3 + 0x98);
    dVar11 = *(double *)(param_3 + 0xb0);
    dVar12 = *(double *)(param_3 + 0xb8);
    dVar13 = *(double *)(param_3 + 0xc0);
    dVar14 = *(double *)(param_3 + 200);
    dVar15 = *(double *)(param_3 + 0xd0);
    lVar26 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x28);
    if (0 < param_2) {
      lVar28 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar28) =
             dVar1 * *(double *)(lVar26 + 8 + lVar28) + *(double *)(lVar22 + 8 + lVar28) * dVar2 +
             *(double *)(lVar24 + 8 + lVar28) * dVar3 + *(double *)(lVar25 + 8 + lVar28) * dVar4 +
             *(double *)(lVar27 + 8 + lVar28) * dVar5 + *(double *)(lVar19 + 8 + lVar28);
        *(double *)(lVar20 + 8 + lVar28) =
             *(double *)(lVar26 + 8 + lVar28) * dVar6 + *(double *)(lVar22 + 8 + lVar28) * dVar7 +
             *(double *)(lVar24 + 8 + lVar28) * dVar8 + *(double *)(lVar25 + 8 + lVar28) * dVar9 +
             *(double *)(lVar27 + 8 + lVar28) * dVar10 + *(double *)(lVar20 + 8 + lVar28);
        *(double *)(lVar21 + 8 + lVar28) =
             *(double *)(lVar26 + 8 + lVar28) * dVar11 + *(double *)(lVar22 + 8 + lVar28) * dVar12 +
             *(double *)(lVar24 + 8 + lVar28) * dVar13 + *(double *)(lVar25 + 8 + lVar28) * dVar14 +
             *(double *)(lVar27 + 8 + lVar28) * dVar15 + *(double *)(lVar21 + 8 + lVar28);
        lVar28 = lVar28 + 8;
      } while (lVar28 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 6:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    lVar25 = *(long *)(param_4 + 0x20);
    lVar26 = *(long *)(param_4 + 0x28);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x68);
    dVar7 = *(double *)(param_3 + 0x78);
    dVar8 = *(double *)(param_3 + 0x80);
    dVar9 = *(double *)(param_3 + 0x88);
    dVar10 = *(double *)(param_3 + 0x90);
    dVar11 = *(double *)(param_3 + 0x98);
    dVar12 = *(double *)(param_3 + 0xa0);
    dVar13 = *(double *)(param_3 + 0xb0);
    dVar14 = *(double *)(param_3 + 0xb8);
    dVar15 = *(double *)(param_3 + 0xc0);
    dVar16 = *(double *)(param_3 + 200);
    dVar17 = *(double *)(param_3 + 0xd0);
    dVar18 = *(double *)(param_3 + 0xd8);
    lVar27 = *(long *)(param_4 + 8);
    lVar28 = *(long *)(param_4 + 0x30);
    if (0 < param_2) {
      lVar23 = 0;
      do {
        *(double *)(lVar19 + 8 + lVar23) =
             dVar1 * *(double *)(lVar27 + 8 + lVar23) + dVar2 * *(double *)(lVar22 + 8 + lVar23) +
             dVar3 * *(double *)(lVar24 + 8 + lVar23) + dVar4 * *(double *)(lVar25 + 8 + lVar23) +
             *(double *)(lVar26 + 8 + lVar23) * dVar5 + *(double *)(lVar28 + 8 + lVar23) * dVar6 +
             *(double *)(lVar19 + 8 + lVar23);
        *(double *)(lVar20 + 8 + lVar23) =
             *(double *)(lVar27 + 8 + lVar23) * dVar7 + *(double *)(lVar22 + 8 + lVar23) * dVar8 +
             *(double *)(lVar24 + 8 + lVar23) * dVar9 + *(double *)(lVar25 + 8 + lVar23) * dVar10 +
             *(double *)(lVar26 + 8 + lVar23) * dVar11 + *(double *)(lVar28 + 8 + lVar23) * dVar12 +
             *(double *)(lVar20 + 8 + lVar23);
        *(double *)(lVar21 + 8 + lVar23) =
             *(double *)(lVar27 + 8 + lVar23) * dVar13 + *(double *)(lVar22 + 8 + lVar23) * dVar14 +
             *(double *)(lVar24 + 8 + lVar23) * dVar15 + *(double *)(lVar25 + 8 + lVar23) * dVar16 +
             *(double *)(lVar26 + 8 + lVar23) * dVar17 + *(double *)(lVar28 + 8 + lVar23) * dVar18 +
             *(double *)(lVar21 + 8 + lVar23);
        lVar23 = lVar23 + 8;
      } while (lVar23 != (ulong)(param_2 - 1) * 8 + 8);
    }
  }
  return;
}


