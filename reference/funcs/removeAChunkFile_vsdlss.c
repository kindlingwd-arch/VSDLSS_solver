/* VSDLSS function (decompiled by Ghidra) */
/* name: removeAChunkFile_vsdlss  addr: 009e2bb0  size: 40 */
#include "vsdlss_ref.h"

/* signature: void removeAChunkFile_vsdlss(undefined8 param_1,undefined4 param_2); */

void removeAChunkFile_vsdlss(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  char acStack_418 [1032];
  
  closeAChunkFile_vsdlss();
  iVar1 = AChunkFileExists_vsdlss(param_1,param_2);
  if (iVar1 != 0) {
    getAChunkFullFn_vsdlss(param_1,param_2,acStack_418);
    remove(acStack_418);
  }
  return;
}


