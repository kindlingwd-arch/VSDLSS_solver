/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDBuildBpqArrUseMacro_vsdlss  addr: 00a28590  size: 1a4 */
#include "vsdlss_ref.h"

/* signature: void MLDBuildBpqArrUseMacro_vsdlss(long param_1,long param_2,long param_3,int *param_4,undefined8 param_5); */

void MLDBuildBpqArrUseMacro_vsdlss
               (long param_1,long param_2,long param_3,int *param_4,undefined8 param_5)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  int *piVar8;
  long lVar9;
  int iVar10;
  
  iVar1 = *(int *)(param_1 + 8);
  lVar9 = *(long *)(param_1 + 0x28);
  if (0 < iVar1) {
    if (*(int *)(param_2 + 4) == -0x7ffffff4) {
      iVar5 = 1;
      piVar8 = (int *)(param_2 + 8);
      do {
        iVar5 = iVar5 + 1;
        if (iVar1 < iVar5) {
          lVar2 = *(long *)(param_4 + 10);
          lVar3 = *(long *)(param_4 + 6);
          lVar4 = *(long *)(param_4 + 8);
          lVar7 = 0;
          iVar5 = 1;
          do {
            iVar10 = *(int *)(lVar9 + 4 + lVar7) + 1;
            piVar8 = (int *)(lVar3 + (long)iVar10 * 4);
            *(int *)(lVar2 + 4 + lVar7) = *piVar8;
            *(int *)(lVar4 + 4 + lVar7) = iVar10 + *param_4;
            *piVar8 = iVar5;
            iVar10 = *(int *)(lVar2 + 4 + lVar7);
            if (param_4[4] != iVar10) {
              *(int *)(lVar4 + (long)iVar10 * 4) = iVar5;
            }
            iVar5 = iVar5 + 1;
            param_4[2] = param_4[2] + 1;
            lVar7 = lVar7 + 4;
          } while (iVar5 != iVar1 + 1);
          return;
        }
        iVar10 = *piVar8;
        piVar8 = piVar8 + 1;
      } while (iVar10 == -0x7ffffff4);
    }
    lVar9 = 4;
    iVar5 = 1;
    do {
      iVar10 = *(int *)(param_2 + lVar9);
      if ((iVar10 < -0x3ffffff9) && (iVar10 != -0x7ffffff5)) {
        if (*(int *)(param_3 + lVar9) < 1) {
          iVar6 = MLDFindDegInElimGraphArr_vsdlss(iVar5,param_2,param_1,param_5);
          iVar6 = iVar6 + (-0x7ffffff4 - iVar10);
        }
        else {
          iVar6 = MLDUpdateNodeAdjArray_vsdlss(iVar5,param_2,param_1,param_3,param_5);
          iVar6 = iVar6 + (-0x7ffffff4 - iVar10);
        }
        lVar2 = *(long *)(param_4 + 10);
        piVar8 = (int *)(*(long *)(param_4 + 6) + (long)(iVar6 + 1) * 4);
        *(int *)(lVar9 + lVar2) = *piVar8;
        lVar3 = *(long *)(param_4 + 8);
        *(int *)(lVar3 + lVar9) = iVar6 + 1 + *param_4;
        *piVar8 = iVar5;
        iVar10 = *(int *)(lVar9 + lVar2);
        if (param_4[4] != iVar10) {
          *(int *)(lVar3 + (long)iVar10 * 4) = iVar5;
        }
        param_4[2] = param_4[2] + 1;
      }
      iVar5 = iVar5 + 1;
      lVar9 = lVar9 + 4;
    } while (iVar5 <= iVar1);
  }
  return;
}


