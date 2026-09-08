/* VSDLSS function (decompiled by Ghidra) */
/* name: removeFactorChunkFile_vsdlss  addr: 009e2b20  size: 40 */
#include "vsdlss_ref.h"

/* signature: void removeFactorChunkFile_vsdlss(undefined8 param_1,undefined4 param_2); */

void removeFactorChunkFile_vsdlss(undefined8 param_1,undefined4 param_2)

{
  int iVar1;
  char acStack_418 [1032];
  
  closeFactorChunkFile_vsdlss();
  iVar1 = factorChunkFileExists_vsdlss(param_1,param_2);
  if (iVar1 != 0) {
    getFactorChunkFullFn_vsdlss(param_1,param_2,acStack_418);
    remove(acStack_418);
  }
  return;
}


