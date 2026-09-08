/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDConvertEdgeSepToNodeSep_vsdlss  addr: 00a02cc0  size: e9 */
#include "vsdlss_ref.h"

/* signature: undefined4 MLDConvertEdgeSepToNodeSep_vsdlss(undefined4 param_1,long param_2,long param_3); */

undefined4 MLDConvertEdgeSepToNodeSep_vsdlss(undefined4 param_1,long param_2,long param_3)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  int *piVar7;
  undefined1 auStack_38 [24];
  
  iVar5 = computePickLighterSideF(param_2,param_3);
  if (iVar5 == 0) {
    lVar2 = *(long *)(param_3 + 0x88);
    *(undefined8 *)(param_3 + 0x88) = 0;
    uVar6 = MLDGetBiPartiteGraph_vsdlss(param_1,param_2,param_3,auStack_38,lVar2);
    lVar3 = *(long *)(param_3 + 0x80);
    *(undefined8 *)(param_3 + 0x80) = 0;
    iVar5 = MLDMinCover_vsdlss(auStack_38,uVar6,param_3,lVar3);
    lVar4 = *(long *)(param_3 + 0xa0);
    if (0 < iVar5) {
      piVar7 = (int *)(lVar3 + 4);
      do {
        iVar1 = *piVar7;
        piVar7 = piVar7 + 1;
        *(undefined4 *)(lVar4 + (long)*(int *)(lVar2 + (long)iVar1 * 4) * 4) = 3;
      } while (piVar7 != (int *)(lVar3 + 8 + (ulong)(iVar5 - 1) * 4));
    }
    *(long *)(param_3 + 0x80) = lVar3;
    *(long *)(param_3 + 0x88) = lVar2;
  }
  else {
    pickLighterSideAsSep(param_2,param_3);
  }
  MLDComputeGraphNodePartitionDetails_1(param_2,param_3);
  MLDFMNodeRefine_h(param_2,param_3);
  return *(undefined4 *)(param_2 + 0x10);
}


