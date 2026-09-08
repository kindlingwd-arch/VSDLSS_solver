/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDGrowRegionBalanceRefine_vsdlss  addr: 00a006f0  size: 2b0 */
#include "vsdlss_ref.h"

/* signature: void MLDGrowRegionBalanceRefine_vsdlss(int param_1,long param_2,long param_3,long param_4,long param_5,long param_6); */

void MLDGrowRegionBalanceRefine_vsdlss
               (int param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  long lVar12;
  int iVar13;
  int *piVar14;
  long lVar15;
  long lVar16;
  char *pcVar17;
  int iVar18;
  
  iVar11 = *(int *)(param_2 + 8);
  lVar2 = *(long *)(param_6 + 0xa0);
  lVar12 = *(long *)(param_2 + 0x28);
  lVar3 = *(long *)(param_6 + 0x48);
  lVar4 = *(long *)(param_2 + 0x58);
  *(undefined8 *)(param_6 + 0x48) = 0;
  initCVector_vsdlss(lVar3,(long)iVar11,0);
  *(undefined4 *)(param_2 + 0x1c) = 0;
  *(int *)(param_2 + 0x20) = *(int *)(param_3 + 4) + *(int *)(param_3 + 8);
  initIVector_vsdlss(lVar2,(long)iVar11,2);
  lVar5 = *(long *)(param_6 + 0x30);
  uVar6 = *(undefined8 *)(param_6 + 0x60);
  *(int *)(lVar5 + 8) = iVar11;
  *(undefined8 *)(lVar5 + 0x10) = uVar6;
  initQ_vsdlss(lVar5);
  lVar7 = *(long *)(lVar5 + 0x10);
  lVar16 = 0;
  iVar10 = *(int *)(lVar5 + 8);
  iVar18 = 2;
  *(int *)(lVar7 + 4) = param_1;
  *(undefined1 *)(lVar3 + param_1) = 1;
  do {
    iVar8 = (int)lVar16;
    iVar9 = *(int *)(lVar7 + 4 + lVar16 * 4);
    lVar15 = (long)iVar9;
    if (lVar4 == 0) {
      iVar9 = 1;
    }
    else {
      iVar9 = *(int *)(lVar4 + (long)iVar9 * 4);
    }
    if ((*(int *)(param_2 + 0x1c) < 1) ||
       (*(int *)(param_5 + 8) <= *(int *)(param_2 + 0x20) - iVar9)) {
      *(undefined4 *)(lVar2 + lVar15 * 4) = 1;
      *(int *)(param_2 + 0x1c) = *(int *)(param_2 + 0x1c) + iVar9;
      iVar9 = *(int *)(param_2 + 0x20) - iVar9;
      *(int *)(param_2 + 0x20) = iVar9;
      if (iVar9 <= *(int *)(param_4 + 8)) break;
      iVar9 = *(int *)(lVar12 + lVar15 * 4);
      piVar14 = *(int **)(*(long *)(param_2 + 0x38) + lVar15 * 8);
      if (0 < iVar9) {
        iVar13 = 1;
        do {
          piVar14 = piVar14 + 1;
          iVar1 = *piVar14;
          pcVar17 = (char *)(iVar1 + lVar3);
          if (*pcVar17 == '\0') {
            if (iVar10 < iVar18) {
              nrerror_vsdlss(1,"mldorder.c",0xaea);
            }
            lVar15 = (long)iVar18;
            iVar18 = iVar18 + 1;
            *(int *)(lVar7 + lVar15 * 4) = iVar1;
            *pcVar17 = '\x01';
          }
          iVar13 = iVar13 + 1;
        } while (iVar13 <= iVar9);
      }
    }
    lVar16 = lVar16 + 1;
  } while (iVar8 + 2 < iVar18);
  setQToNull_vsdlss(lVar5);
  *(long *)(param_6 + 0x48) = lVar3;
  if (*(int *)(param_2 + 0x20) == 0) {
    iVar10 = rand();
    lVar12 = (long)((iVar10 >> 3) % iVar11 + 1);
    *(undefined4 *)(lVar2 + lVar12 * 4) = 2;
    if (lVar4 == 0) {
      iVar11 = 1;
    }
    else {
      iVar11 = *(int *)(lVar4 + lVar12 * 4);
    }
    *(int *)(param_2 + 0x20) = *(int *)(param_2 + 0x20) + iVar11;
    *(int *)(param_2 + 0x1c) = *(int *)(param_2 + 0x1c) - iVar11;
  }
  computeInterVecsForBalanceRefine_vsdlss(param_3,param_2,param_6);
  MLDBalance_vsdlss(param_3,param_2,param_6);
  MLDFMRefine_h(param_3,param_2,param_6);
  return;
}


