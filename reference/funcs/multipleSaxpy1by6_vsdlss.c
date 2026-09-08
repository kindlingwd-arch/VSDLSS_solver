/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSaxpy1by6_vsdlss  addr: 00a1bf40  size: 32b */
#include "vsdlss_ref.h"

/* signature: void multipleSaxpy1by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5); */

void multipleSaxpy1by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  
  lVar7 = *(long *)(param_5 + 8);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x89,6,"multipleSaxpy1by6");
    return;
  case 1:
    dVar1 = *(double *)(param_3 + 0x40);
    lVar8 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      lVar10 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar10) =
             *(double *)(lVar8 + 8 + lVar10) * dVar1 + *(double *)(lVar7 + 8 + lVar10);
        lVar10 = lVar10 + 8;
      } while (lVar10 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 2:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    if (0 < param_2) {
      lVar11 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar11) =
             *(double *)(lVar8 + 8 + lVar11) * dVar1 + *(double *)(lVar10 + 8 + lVar11) * dVar2 +
             *(double *)(lVar7 + 8 + lVar11);
        lVar11 = lVar11 + 8;
      } while (lVar11 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 3:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar11 = *(long *)(param_4 + 0x18);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    if (0 < param_2) {
      lVar13 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar13) =
             *(double *)(lVar8 + 8 + lVar13) * dVar1 + *(double *)(lVar10 + 8 + lVar13) * dVar2 +
             *(double *)(lVar11 + 8 + lVar13) * dVar3 + *(double *)(lVar7 + 8 + lVar13);
        lVar13 = lVar13 + 8;
      } while (lVar13 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 4:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar13 = *(long *)(param_4 + 0x20);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    if (0 < param_2) {
      lVar12 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar12) =
             *(double *)(lVar8 + 8 + lVar12) * dVar1 + *(double *)(lVar10 + 8 + lVar12) * dVar2 +
             *(double *)(lVar11 + 8 + lVar12) * dVar3 + *(double *)(lVar13 + 8 + lVar12) * dVar4 +
             *(double *)(lVar7 + 8 + lVar12);
        lVar12 = lVar12 + 8;
      } while (lVar12 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 5:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar12 = *(long *)(param_4 + 0x28);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    if (0 < param_2) {
      lVar14 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar14) =
             *(double *)(lVar8 + 8 + lVar14) * dVar1 + *(double *)(lVar10 + 8 + lVar14) * dVar2 +
             *(double *)(lVar11 + 8 + lVar14) * dVar3 + *(double *)(lVar13 + 8 + lVar14) * dVar4 +
             *(double *)(lVar12 + 8 + lVar14) * dVar5 + *(double *)(lVar7 + 8 + lVar14);
        lVar14 = lVar14 + 8;
      } while (lVar14 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 6:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    lVar12 = *(long *)(param_4 + 0x28);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar14 = *(long *)(param_4 + 0x30);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x68);
    if (0 < param_2) {
      lVar9 = 0;
      do {
        *(double *)(lVar7 + 8 + lVar9) =
             *(double *)(lVar8 + 8 + lVar9) * dVar1 + *(double *)(lVar10 + 8 + lVar9) * dVar2 +
             *(double *)(lVar11 + 8 + lVar9) * dVar3 + *(double *)(lVar13 + 8 + lVar9) * dVar4 +
             *(double *)(lVar12 + 8 + lVar9) * dVar5 + *(double *)(lVar14 + 8 + lVar9) * dVar6 +
             *(double *)(lVar7 + 8 + lVar9);
        lVar9 = lVar9 + 8;
      } while (lVar9 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
  }
  return;
}


