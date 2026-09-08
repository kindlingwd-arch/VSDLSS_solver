/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDRemFromBucketMarkInBpqNodesInArr_vsdlss  addr: 00a2ac70  size: d2 */
#include "vsdlss_ref.h"

/* signature: void MLDRemFromBucketMarkInBpqNodesInArr_vsdlss(int param_1,long param_2,long param_3,uint *param_4); */

void MLDRemFromBucketMarkInBpqNodesInArr_vsdlss(int param_1,long param_2,long param_3,uint *param_4)

{
  uint *puVar1;
  long lVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  long lVar9;
  int *piVar10;
  uint *puVar11;
  
  if (0 < param_1) {
    piVar10 = (int *)(param_2 + 4);
    piVar3 = (int *)(param_2 + 8 + (ulong)(param_1 - 1) * 4);
    do {
      while( true ) {
        iVar4 = *piVar10;
        lVar2 = (long)iVar4 * 4;
        if (*(int *)(param_3 + (long)iVar4 * 4) < 1) break;
LAB_00a2ac90:
        piVar10 = piVar10 + 1;
        if (piVar10 == piVar3) {
          return;
        }
      }
      lVar9 = *(long *)(param_4 + 8);
      uVar5 = *param_4;
      uVar6 = param_4[4];
      puVar1 = (uint *)(lVar9 + lVar2);
      uVar7 = *puVar1;
      if (((int)uVar7 < 1) || ((int)(uVar5 * 2) < (int)uVar7)) goto LAB_00a2ac90;
      puVar11 = (uint *)(lVar2 + *(long *)(param_4 + 10));
      uVar8 = *puVar11;
      if ((int)uVar5 < (int)uVar7) {
        *(uint *)(*(long *)(param_4 + 6) + (long)(int)(uVar7 - uVar5) * 4) = uVar8;
      }
      else {
        *(uint *)(*(long *)(param_4 + 10) + (long)(int)uVar7 * 4) = uVar8;
      }
      if (uVar8 != uVar6) {
        *(uint *)(lVar9 + (long)(int)uVar8 * 4) = uVar7;
      }
      param_4[2] = param_4[2] - 1;
      *puVar11 = param_4[5];
      *puVar1 = ~uVar5;
      param_4[5] = -iVar4;
      if (*puVar11 != uVar6) {
        *(int *)(lVar9 + (long)(int)-*puVar11 * 4) = -iVar4;
      }
      piVar10 = piVar10 + 1;
      param_4[3] = param_4[3] + 1;
    } while (piVar10 != piVar3);
  }
  return;
}


