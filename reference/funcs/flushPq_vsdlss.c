/* VSDLSS function (decompiled by Ghidra) */
/* name: flushPq_vsdlss  addr: 00a2cb50  size: 2a */
#include "vsdlss_ref.h"

/* signature: void flushPq_vsdlss(long param_1); */

void flushPq_vsdlss(long param_1)

{
  int iVar1;
  undefined1 local_c [4];
  
  iVar1 = *(int *)(param_1 + 8);
  while (iVar1 != 0) {
    dqPq_vsdlss(param_1,local_c);
    iVar1 = *(int *)(param_1 + 8);
  }
  return;
}


