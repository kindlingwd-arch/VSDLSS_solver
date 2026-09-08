/* VSDLSS function (decompiled by Ghidra) */
/* name: multipleSaxpy2by6_vsdlss  addr: 00a1c2a0  size: 554 */
#include "vsdlss_ref.h"

/* signature: void multipleSaxpy2by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5); */

void multipleSaxpy2by6_vsdlss(undefined4 param_1,int param_2,long param_3,long param_4,long param_5)

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
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  
  lVar13 = *(long *)(param_5 + 8);
  lVar14 = *(long *)(param_5 + 0x10);
  switch(param_1) {
  default:
    nrerror_vsdlss(0xf,"arithkernels.c",0x133,6,"multipleSaxpy2by6");
    return;
  case 1:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x78);
    lVar15 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      lVar17 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar17) =
             *(double *)(lVar15 + 8 + lVar17) * dVar1 + *(double *)(lVar13 + 8 + lVar17);
        *(double *)(lVar14 + 8 + lVar17) =
             *(double *)(lVar15 + 8 + lVar17) * dVar2 + *(double *)(lVar14 + 8 + lVar17);
        lVar17 = lVar17 + 8;
      } while (lVar17 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 2:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x78);
    dVar4 = *(double *)(param_3 + 0x80);
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    if (0 < param_2) {
      lVar18 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar18) =
             *(double *)(lVar15 + 8 + lVar18) * dVar1 + *(double *)(lVar17 + 8 + lVar18) * dVar2 +
             *(double *)(lVar13 + 8 + lVar18);
        *(double *)(lVar14 + 8 + lVar18) =
             *(double *)(lVar15 + 8 + lVar18) * dVar3 + *(double *)(lVar17 + 8 + lVar18) * dVar4 +
             *(double *)(lVar14 + 8 + lVar18);
        lVar18 = lVar18 + 8;
      } while (lVar18 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 3:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x78);
    dVar5 = *(double *)(param_3 + 0x80);
    dVar6 = *(double *)(param_3 + 0x88);
    lVar17 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x18);
    if (0 < param_2) {
      lVar19 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar19) =
             *(double *)(lVar17 + 8 + lVar19) * dVar1 + *(double *)(lVar15 + 8 + lVar19) * dVar2 +
             *(double *)(lVar18 + 8 + lVar19) * dVar3 + *(double *)(lVar13 + 8 + lVar19);
        *(double *)(lVar14 + 8 + lVar19) =
             *(double *)(lVar17 + 8 + lVar19) * dVar4 + *(double *)(lVar15 + 8 + lVar19) * dVar5 +
             *(double *)(lVar18 + 8 + lVar19) * dVar6 + *(double *)(lVar14 + 8 + lVar19);
        lVar19 = lVar19 + 8;
      } while (lVar19 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 4:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar17 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x78);
    dVar6 = *(double *)(param_3 + 0x80);
    dVar7 = *(double *)(param_3 + 0x88);
    dVar8 = *(double *)(param_3 + 0x90);
    lVar18 = *(long *)(param_4 + 8);
    lVar19 = *(long *)(param_4 + 0x20);
    if (0 < param_2) {
      lVar20 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar20) =
             *(double *)(lVar18 + 8 + lVar20) * dVar1 + *(double *)(lVar15 + 8 + lVar20) * dVar2 +
             *(double *)(lVar17 + 8 + lVar20) * dVar3 + *(double *)(lVar19 + 8 + lVar20) * dVar4 +
             *(double *)(lVar13 + 8 + lVar20);
        *(double *)(lVar14 + 8 + lVar20) =
             *(double *)(lVar18 + 8 + lVar20) * dVar5 + *(double *)(lVar15 + 8 + lVar20) * dVar6 +
             *(double *)(lVar17 + 8 + lVar20) * dVar7 + *(double *)(lVar19 + 8 + lVar20) * dVar8 +
             *(double *)(lVar14 + 8 + lVar20);
        lVar20 = lVar20 + 8;
      } while (lVar20 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 5:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar17 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    lVar18 = *(long *)(param_4 + 0x20);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x78);
    dVar7 = *(double *)(param_3 + 0x80);
    dVar8 = *(double *)(param_3 + 0x88);
    dVar9 = *(double *)(param_3 + 0x90);
    dVar10 = *(double *)(param_3 + 0x98);
    lVar19 = *(long *)(param_4 + 8);
    lVar20 = *(long *)(param_4 + 0x28);
    if (0 < param_2) {
      lVar21 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar21) =
             *(double *)(lVar19 + 8 + lVar21) * dVar1 + *(double *)(lVar15 + 8 + lVar21) * dVar2 +
             *(double *)(lVar17 + 8 + lVar21) * dVar3 + *(double *)(lVar18 + 8 + lVar21) * dVar4 +
             *(double *)(lVar20 + 8 + lVar21) * dVar5 + *(double *)(lVar13 + 8 + lVar21);
        *(double *)(lVar14 + 8 + lVar21) =
             *(double *)(lVar19 + 8 + lVar21) * dVar6 + *(double *)(lVar15 + 8 + lVar21) * dVar7 +
             *(double *)(lVar17 + 8 + lVar21) * dVar8 + *(double *)(lVar18 + 8 + lVar21) * dVar9 +
             *(double *)(lVar20 + 8 + lVar21) * dVar10 + *(double *)(lVar14 + 8 + lVar21);
        lVar21 = lVar21 + 8;
      } while (lVar21 != (ulong)(param_2 - 1) * 8 + 8);
      return;
    }
    break;
  case 6:
    lVar15 = *(long *)(param_4 + 0x10);
    lVar17 = *(long *)(param_4 + 0x18);
    lVar18 = *(long *)(param_4 + 0x20);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar19 = *(long *)(param_4 + 0x28);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x68);
    dVar7 = *(double *)(param_3 + 0x78);
    dVar8 = *(double *)(param_3 + 0x80);
    dVar9 = *(double *)(param_3 + 0x88);
    dVar10 = *(double *)(param_3 + 0x90);
    dVar11 = *(double *)(param_3 + 0x98);
    dVar12 = *(double *)(param_3 + 0xa0);
    lVar20 = *(long *)(param_4 + 8);
    lVar21 = *(long *)(param_4 + 0x30);
    if (0 < param_2) {
      lVar16 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar16) =
             *(double *)(lVar20 + 8 + lVar16) * dVar1 + *(double *)(lVar15 + 8 + lVar16) * dVar2 +
             *(double *)(lVar17 + 8 + lVar16) * dVar3 + *(double *)(lVar18 + 8 + lVar16) * dVar4 +
             *(double *)(lVar19 + 8 + lVar16) * dVar5 + *(double *)(lVar21 + 8 + lVar16) * dVar6 +
             *(double *)(lVar13 + 8 + lVar16);
        *(double *)(lVar14 + 8 + lVar16) =
             *(double *)(lVar20 + 8 + lVar16) * dVar7 + *(double *)(lVar15 + 8 + lVar16) * dVar8 +
             *(double *)(lVar17 + 8 + lVar16) * dVar9 + *(double *)(lVar18 + 8 + lVar16) * dVar10 +
             *(double *)(lVar19 + 8 + lVar16) * dVar11 + *(double *)(lVar21 + 8 + lVar16) * dVar12 +
             *(double *)(lVar14 + 8 + lVar16);
        lVar16 = lVar16 + 8;
      } while (lVar16 != (ulong)(param_2 - 1) * 8 + 8);
    }
  }
  return;
}


