/* VSDLSS function (decompiled by Ghidra) */
/* name: getMldovsBfsQ_vsdlss  addr: 009fce50  size: 1d */
#include "vsdlss_ref.h"

/* signature: long getMldovsBfsQ_vsdlss(undefined4 param_1,long param_2); */

long getMldovsBfsQ_vsdlss(undefined4 param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_2 + 0x30);
  uVar2 = *(undefined8 *)(param_2 + 0x60);
  *(undefined4 *)(lVar1 + 8) = param_1;
  *(undefined8 *)(lVar1 + 0x10) = uVar2;
  initQ_vsdlss(lVar1);
  return lVar1;
}


