/* VSDLSS function (decompiled by Ghidra) */
/* name: newArrBucketArr_vsdlss  addr: 00a1e1d0  size: 9d */
#include "vsdlss_ref.h"

/* signature: undefined4 * newArrBucketArr_vsdlss(int param_1); */

undefined4 * newArrBucketArr_vsdlss(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x20);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  lVar3 = newBlock_vsdlss(uVar1,(long)(param_1 + 1) << 3);
  *(long *)(puVar2 + 2) = lVar3;
  if (0 < param_1) {
    lVar5 = 8;
    while( true ) {
      *(undefined8 *)(lVar3 + lVar5) = 0;
      lVar5 = lVar5 + 8;
      if (lVar5 == (ulong)(param_1 - 1) * 8 + 0x10) break;
      lVar3 = *(long *)(puVar2 + 2);
    }
  }
  uVar4 = tagNewIVector_vsdlss(uVar1,(long)param_1,1,0);
  *(undefined8 *)(puVar2 + 4) = uVar4;
  uVar4 = tagNewIVector_vsdlss(uVar1,(long)param_1,1,0);
  *(undefined8 *)(puVar2 + 6) = uVar4;
  return puVar2;
}


