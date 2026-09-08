/* VSDLSS function (decompiled by Ghidra) */
/* name: computeInterVecsForBalanceRefine_vsdlss  addr: 009fef80  size: 358 */
#include "vsdlss_ref.h"

/* signature: void computeInterVecsForBalanceRefine_vsdlss(undefined8 param_1,long param_2,long param_3); */

void computeInterVecsForBalanceRefine_vsdlss(undefined8 param_1,long param_2,long param_3)

{
  long lVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  int *piVar15;
  int *piVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  int iVar20;
  uint uVar21;
  int iVar22;
  long lVar23;
  
  iVar19 = *(int *)(param_2 + 8);
  lVar6 = *(long *)(param_3 + 0x90);
  piVar16 = *(int **)(param_3 + 0x98);
  lVar7 = *(long *)(param_2 + 0x58);
  lVar8 = *(long *)(param_2 + 0x28);
  lVar9 = *(long *)(param_3 + 0xa8);
  lVar23 = (long)iVar19;
  iVar22 = *(int *)(param_2 + 4);
  lVar10 = *(long *)(param_3 + 0xa0);
  lVar11 = *(long *)(param_3 + 0xb0);
  lVar12 = *(long *)(param_2 + 0x38);
  lVar18 = *(long *)(param_2 + 0x48);
  initIVector_vsdlss(lVar6,lVar23,0);
  initIVector_vsdlss(piVar16,lVar23,0);
  initIVector_vsdlss(lVar9,lVar23,0);
  *(undefined4 *)(param_2 + 0x1c) = 0;
  *(undefined4 *)(param_2 + 0x20) = 0;
  if ((lVar18 == 0) && (iVar22 == 1)) {
    if (iVar19 < 1) {
LAB_009ff2be:
      *(undefined4 *)(param_2 + 0x14) = 0;
      *(undefined4 *)(param_2 + 0x10) = 0;
      return;
    }
    lVar18 = 0;
    uVar21 = 0;
    iVar22 = 0;
    do {
      piVar16 = piVar16 + 1;
      iVar20 = *(int *)(lVar10 + 4 + lVar18 * 4);
      lVar23 = *(long *)(lVar12 + 8 + lVar18 * 8);
      piVar15 = (int *)(param_2 + 0x18 + (long)iVar20 * 4);
      *piVar15 = *piVar15 + 1;
      iVar4 = *(int *)(lVar8 + 4 + lVar18 * 4);
      if (0 < iVar4) {
        piVar15 = (int *)(lVar23 + 4);
        piVar2 = (int *)(lVar23 + 8 + (ulong)(iVar4 - 1) * 4);
        do {
          while (*(int *)(lVar10 + (long)*piVar15 * 4) == iVar20) {
            piVar15 = piVar15 + 1;
            piVar3 = (int *)(lVar6 + 4 + lVar18 * 4);
            *piVar3 = *piVar3 + 1;
            if (piVar15 == piVar2) goto LAB_009ff0d3;
          }
          piVar15 = piVar15 + 1;
          *piVar16 = *piVar16 + 1;
        } while (piVar15 != piVar2);
      }
LAB_009ff0d3:
      if (0 < *piVar16) {
        iVar22 = iVar22 + 1;
        uVar21 = uVar21 + *piVar16;
        *(int *)(lVar11 + (long)iVar22 * 4) = (int)lVar18 + 1;
        *(int *)(lVar9 + 4 + lVar18 * 4) = iVar22;
      }
      lVar18 = lVar18 + 1;
    } while (lVar18 != (ulong)(iVar19 - 1) + 1);
  }
  else {
    if (iVar19 < 1) goto LAB_009ff2be;
    lVar23 = 0;
    iVar20 = 1;
    uVar21 = 0;
    iVar22 = 0;
    do {
      piVar16 = piVar16 + 1;
      iVar4 = *(int *)(lVar10 + 4 + lVar23);
      piVar15 = (int *)(param_2 + 0x18 + (long)iVar4 * 4);
      *piVar15 = *piVar15 + *(int *)(lVar7 + 4 + lVar23);
      lVar13 = *(long *)(lVar12 + 8 + lVar23 * 2);
      iVar5 = *(int *)(lVar8 + 4 + lVar23);
      lVar14 = *(long *)(lVar18 + 8 + lVar23 * 2);
      if (0 < iVar5) {
        lVar1 = (ulong)(iVar5 - 1) * 4 + 4;
        lVar17 = 0;
        do {
          while (iVar5 = *(int *)(lVar14 + 4 + lVar17),
                *(int *)(lVar10 + (long)*(int *)(lVar13 + 4 + lVar17) * 4) == iVar4) {
            lVar17 = lVar17 + 4;
            piVar15 = (int *)(lVar6 + 4 + lVar23);
            *piVar15 = *piVar15 + iVar5;
            if (lVar17 == lVar1) goto LAB_009ff248;
          }
          lVar17 = lVar17 + 4;
          *piVar16 = *piVar16 + iVar5;
        } while (lVar17 != lVar1);
      }
LAB_009ff248:
      if (0 < *piVar16) {
        iVar22 = iVar22 + 1;
        uVar21 = uVar21 + *piVar16;
        *(int *)(lVar11 + (long)iVar22 * 4) = iVar20;
        *(int *)(lVar9 + 4 + lVar23) = iVar22;
      }
      iVar20 = iVar20 + 1;
      lVar23 = lVar23 + 4;
    } while (iVar20 != iVar19 + 1);
  }
  if ((uVar21 & 1) == 0) {
    iVar20 = *(int *)(param_2 + 0x1c) + *(int *)(param_2 + 0x20);
  }
  else {
    nrerror_vsdlss(1,"mldorder.c",0x87f);
    iVar20 = *(int *)(param_2 + 0x1c) + *(int *)(param_2 + 0x20);
  }
  *(int *)(param_2 + 0x14) = (int)uVar21 >> 1;
  *(int *)(param_2 + 0x10) = iVar22;
  if (iVar19 < 1) {
    iVar19 = 0;
  }
  else {
    piVar16 = (int *)(lVar7 + 4);
    piVar15 = (int *)(lVar7 + 8 + (ulong)(iVar19 - 1) * 4);
    iVar19 = 0;
    do {
      while (lVar7 == 0) {
        piVar16 = piVar16 + 1;
        iVar19 = iVar19 + 1;
        if (piVar16 == piVar15) goto LAB_009ff178;
      }
      iVar22 = *piVar16;
      piVar16 = piVar16 + 1;
      iVar19 = iVar19 + iVar22;
    } while (piVar16 != piVar15);
  }
LAB_009ff178:
  if (iVar19 == iVar20) {
    return;
  }
  nrerror_vsdlss(1,"mldorder.c",0x88c);
  return;
}


