/* VSDLSS function (decompiled by Ghidra) */
/* name: tagRead_DEG123REORDERVEC_vsdlss  addr: 00a0e420  size: 36 */
#include "vsdlss_ref.h"

/* signature: undefined8 tagRead_DEG123REORDERVEC_vsdlss(undefined4 param_1); */

undefined8 tagRead_DEG123REORDERVEC_vsdlss(undefined4 param_1)

{
  undefined8 uVar1;
  undefined1 local_c [4];
  
  uVar1 = openFileR_vsdlss(0x10);
  uVar1 = tagReadIVector_vsdlss(param_1,uVar1,local_c);
  casiFClose_vsdlss(0x10);
  return uVar1;
}


