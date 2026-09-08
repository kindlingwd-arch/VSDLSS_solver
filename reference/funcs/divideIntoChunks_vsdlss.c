/* VSDLSS function (decompiled by Ghidra) */
/* name: divideIntoChunks_vsdlss  addr: 00a1f7b0  size: 1a5 */
#include "vsdlss_ref.h"

/* signature: undefined8 divideIntoChunks_vsdlss(undefined8 param_1,undefined4 param_2,undefined8 param_3,uint *param_4,undefined4 *param_5,undefined8 param_6,undefined8 param_7,undefined8 *param_8,undefined8 *param_9); */

undefined8
divideIntoChunks_vsdlss
          (undefined8 param_1,undefined4 param_2,undefined8 param_3,uint *param_4,
          undefined4 *param_5,undefined8 param_6,undefined8 param_7,undefined8 *param_8,
          undefined8 *param_9)

{
  uint uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 uVar5;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40 [2];
  
  uVar1 = *param_4;
  uVar2 = newIVector_vsdlss((long)(int)uVar1,1,0);
  uVar3 = newIVector_vsdlss((long)(int)uVar1,1,0);
  lVar4 = newCEdgeVector_vsdlss(uVar1,1,0);
  debugPrint_vsdlss("Start divideIntoChunks\n");
  uVar5 = newStack_vsdlss(~uVar1);
  local_48 = 0;
  local_44 = 1;
  local_40[0] = 0;
  for (; param_5 != (undefined4 *)0x0; param_5 = *(undefined4 **)(param_5 + 2)) {
    modifiedDfs(param_1,*param_5,param_4,param_6,param_7,uVar3,&local_48,&local_44,local_40,uVar2,
                uVar5,lVar4);
  }
  deleteStack_vsdlss(uVar5);
  getInverseMapInPlace_vsdlss(uVar3,uVar1);
  permuteIVectorInPlace_vsdlss(uVar3,uVar1,uVar2);
  checkNodeToChunkVec(local_44,uVar2,uVar1);
  debugPrint_vsdlss("Chunks obtained\n");
  uVar5 = createChunkInfo(param_2,param_3,param_7,uVar3,uVar2);
  debugPrint_vsdlss("ChunkInfo created\n");
  if (lVar4 != 0) {
    casiFree_vsdlss(lVar4);
  }
  debugPrint_vsdlss("End divideIntoChunks\n");
  *param_8 = uVar3;
  *param_9 = uVar2;
  return uVar5;
}


