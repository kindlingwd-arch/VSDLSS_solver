/* VSDLSS function (decompiled by Ghidra) */
/* name: newChunkInfo_vsdlss  addr: 009e22d0  size: 175 */
#include "vsdlss_ref.h"

/* signature: undefined1 * newChunkInfo_vsdlss(undefined4 param_1,int param_2,int param_3,int param_4); */

undefined1 * newChunkInfo_vsdlss(undefined4 param_1,int param_2,int param_3,int param_4)

{
  undefined1 *puVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  long lVar5;
  
  puVar1 = (undefined1 *)casiMalloc_vsdlss(0x70);
  *(undefined4 *)(puVar1 + 4) = param_1;
  *(int *)(puVar1 + 0x10) = param_4;
  *puVar1 = 0;
  *(int *)(puVar1 + 8) = param_2;
  *(int *)(puVar1 + 0xc) = param_3;
  lVar2 = newCPVector_vsdlss((long)param_3,1,0);
  *(long *)(puVar1 + 0x18) = lVar2;
  lVar5 = 0;
  if (0 < param_3) {
    while( true ) {
      uVar3 = newCVector_vsdlss((long)(param_4 + 1),1,0);
      *(undefined8 *)(lVar2 + (lVar5 + 1) * 8) = uVar3;
      if (param_3 < (int)lVar5 + 2) break;
      lVar2 = *(long *)(puVar1 + 0x18);
      lVar5 = lVar5 + 1;
    }
  }
  lVar2 = (long)param_2;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x20) = uVar3;
  uVar3 = newFilePtrVector_vsdlss(lVar2,1,0);
  *(undefined8 *)(puVar1 + 0x28) = uVar3;
  uVar3 = newFilePtrVector_vsdlss(lVar2,1,0);
  *(undefined8 *)(puVar1 + 0x30) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x38) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x40) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x48) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x50) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x58) = uVar3;
  uVar3 = newIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x60) = uVar3;
  uVar3 = newLIVector_vsdlss(lVar2,0,0);
  *(undefined8 *)(puVar1 + 0x68) = uVar3;
  if (0 < param_2) {
    lVar2 = *(long *)(puVar1 + 0x20);
    uVar4 = 0;
    do {
      *(int *)(lVar2 + 4 + uVar4 * 4) =
           (int)((long)((ulong)(uint)((int)uVar4 >> 0x1f) << 0x20 | uVar4 & 0xffffffff) %
                (long)param_3) + 1;
      uVar4 = uVar4 + 1;
    } while (uVar4 != (ulong)(param_2 - 1) + 1);
  }
  *puVar1 = 1;
  return puVar1;
}


