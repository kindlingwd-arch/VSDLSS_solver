/* VSDLSS function (decompiled by Ghidra) */
/* name: symbolicElimination_vsdlss  addr: 00a2d1a0  size: 249 */
#include "vsdlss_ref.h"

/* signature: undefined8 symbolicElimination_vsdlss(undefined8 *param_1,long param_2,long param_3,long param_4,undefined8 param_5,long param_6); */

undefined8
symbolicElimination_vsdlss
          (undefined8 *param_1,long param_2,long param_3,long param_4,undefined8 param_5,
          long param_6)

{
  int iVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 uVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  long lVar12;
  int local_78;
  undefined8 local_70;
  int *local_40 [2];
  
  local_40[0] = (int *)*param_1;
  iVar1 = *local_40[0];
  uVar5 = newAffectedByChunk_vsdlss(*(undefined4 *)(param_3 + 8));
  lVar6 = newIVector_vsdlss((long)iVar1,0,0);
  debugPrint_vsdlss("Start symbolicElimination\n");
  permuteIVectorInPlace_vsdlss(param_5,iVar1,param_4);
  permuteGraph_vsdlss(local_40,param_5);
  convertG2GLT_vsdlss(local_40[0]);
  convertGLT2GUT_vsdlss(local_40[0]);
  local_70 = openFactorChunkFileW_vsdlss(param_3,1);
  debugPrint_vsdlss("symbolicElimination initialization complete\n");
  if (iVar1 < 1) {
    local_78 = 1;
  }
  else {
    lVar9 = 8;
    lVar12 = 0;
    local_78 = 1;
    iVar11 = 1;
    iVar8 = 1;
    do {
      if (*(int *)(param_6 + 4 + lVar12) != local_78) {
        closeFactorChunkFile_vsdlss(param_3,local_78);
        local_78 = local_78 + 1;
        local_70 = openFactorChunkFileW_vsdlss(param_3,local_78);
      }
      lVar10 = (long)iVar11;
      if (*(int *)(*(long *)(param_2 + 8) + lVar10 * 4) == iVar8) {
        uVar4 = formAndWriteBlockIndVec
                          (iVar8,iVar11,*(undefined4 *)(param_4 + 4 + lVar12),
                           *(undefined8 *)(*(long *)(local_40[0] + 6) + lVar9),local_70,lVar6);
        *(undefined4 *)(*(long *)(param_2 + 0x18) + lVar10 * 4) = uVar4;
      }
      puVar2 = *(undefined4 **)(*(long *)(local_40[0] + 6) + lVar9);
      setAffectedByChunk(iVar8,puVar2,param_6,uVar5);
      if (puVar2 != (undefined4 *)0x0) {
        checkBlockIntegrityAndMerge(iVar8,*puVar2,iVar11,param_2,local_40[0]);
      }
      iVar8 = iVar8 + 1;
      lVar3 = *(long *)(local_40[0] + 6);
      uVar7 = deleteEntireCEdgelist_vsdlss(*(undefined8 *)(lVar9 + lVar3));
      *(undefined8 *)(lVar9 + lVar3) = uVar7;
      lVar12 = lVar12 + 4;
      lVar9 = lVar9 + 8;
      iVar11 = iVar11 + (uint)(*(int *)(*(long *)(param_2 + 8) + lVar10 * 4) +
                               *(int *)(*(long *)(param_2 + 0x10) + lVar10 * 4) <= iVar8);
    } while (iVar8 <= iVar1);
  }
  closeFactorChunkFile_vsdlss(param_3,local_78);
  if (lVar6 != 0) {
    casiFree_vsdlss();
  }
  debugPrint_vsdlss("End symbolicElimination\n");
  *param_1 = local_40[0];
  return uVar5;
}


