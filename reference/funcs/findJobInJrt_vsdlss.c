/* VSDLSS function (decompiled by Ghidra) */
/* name: findJobInJrt_vsdlss  addr: 009f61e0  size: 1a */
#include "vsdlss_ref.h"

/* signature: int findJobInJrt_vsdlss(void); */

int findJobInJrt_vsdlss(void)

{
  int iVar1;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 != -1) {
    return iVar1;
  }
  FUN_009f60f0();
  return -1;
}


