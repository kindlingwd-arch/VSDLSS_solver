/* VSDLSS function (decompiled by Ghidra) */
/* name: computeNodeToThread_vsdlss  addr: 00a08ed0  size: 229 */
#include "vsdlss_ref.h"

/* signature: int computeNodeToThread_vsdlss(int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,long param_7,long param_8,long param_9); */

int computeNodeToThread_vsdlss
              (int param_1,int param_2,int param_3,int param_4,long param_5,long param_6,
              long param_7,long param_8,long param_9)

{
  int iVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  ulong uVar5;
  long lVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  ulong uVar10;
  ulong *puVar11;
  int *piVar12;
  long lVar13;
  ulong auStack_248 [67];
  
  iVar7 = param_1 + param_2 + param_3;
  piVar12 = (int *)(*(long *)(param_5 + 0x10) + (long)*(int *)(param_5 + 4) * 0x28);
  iVar1 = *piVar12;
  iVar3 = piVar12[6];
  lVar13 = *(long *)(piVar12 + 8);
  uVar10 = (ulong)iVar1;
  if (uVar10 != 0) {
    uVar5 = 1;
    do {
      auStack_248[uVar5] = (ulong)((long)param_3 * 3 + (long)param_1 + (long)param_2 * 2) / uVar10;
      uVar5 = uVar5 + 1;
    } while (uVar5 <= uVar10);
  }
  initIVector_vsdlss(param_8,(long)param_4,0);
  iVar3 = iVar7 + iVar3;
  if (0 < iVar3) {
    lVar6 = (long)iVar3;
    piVar12 = (int *)((lVar6 - iVar7) * 0x20 + lVar13);
    lVar13 = 0;
    do {
      if (iVar1 < 2) {
        lVar4 = 1;
      }
      else {
        puVar11 = auStack_248 + 2;
        iVar8 = 2;
        lVar4 = 1;
        uVar10 = auStack_248[1];
        do {
          if (uVar10 < *puVar11) {
            lVar4 = (long)iVar8;
            uVar10 = *puVar11;
          }
          iVar8 = iVar8 + 1;
          puVar11 = puVar11 + 1;
        } while (iVar8 != iVar1 + 1);
      }
      iVar8 = (int)lVar4;
      if (iVar7 < iVar3) {
        iVar2 = piVar12[1];
        iVar9 = *piVar12 + iVar7;
        if (iVar9 <= iVar2 + iVar7) {
          uVar10 = auStack_248[iVar8];
          lVar4 = 0;
          do {
            uVar10 = uVar10 - *(long *)(param_7 + (long)iVar9 * 8 + lVar4 * 2);
            *(int *)(param_8 + (long)iVar9 * 4 + lVar4) = iVar8;
            lVar4 = lVar4 + 4;
          } while (lVar4 != (ulong)(uint)((iVar2 + iVar7) - iVar9) * 4 + 4);
          auStack_248[iVar8] = uVar10;
        }
      }
      else if (*(int *)(param_6 + lVar6 * 4 + lVar13) == iVar3) {
        uVar10 = auStack_248[lVar4];
        *(int *)(lVar6 * 4 + param_8 + lVar13) = iVar8;
        auStack_248[lVar4] = uVar10 - *(long *)(param_7 + lVar6 * 8 + lVar13 * 2);
      }
      lVar13 = lVar13 + -4;
      piVar12 = piVar12 + -8;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  lVar13 = 0;
  if (0 < iVar7) {
    do {
      piVar12 = (int *)(param_8 + (long)*(int *)(param_6 + 4 + lVar13 * 4) * 4);
      iVar3 = *piVar12;
      if (iVar3 == 0) {
        nrerror_vsdlss(1,"prepostpar.c",0xe6);
        iVar3 = *piVar12;
      }
      *(int *)(param_9 + 4 + lVar13 * 4) = iVar3;
      iVar3 = (int)lVar13;
      lVar13 = lVar13 + 1;
    } while (iVar3 + 2 <= iVar7);
  }
  return iVar1;
}


