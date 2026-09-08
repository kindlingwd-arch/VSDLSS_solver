/* VSDLSS function (decompiled by Ghidra) */
/* name: tagReadReorderVec_vsdlss  addr: 00a0e560  size: 36 */
#include "vsdlss_ref.h"

/* signature: undefined8 tagReadReorderVec_vsdlss(undefined4 param_1); */

undefined8 tagReadReorderVec_vsdlss(undefined4 param_1)

{
  undefined8 uVar1;
  undefined1 local_c [4];
  
  uVar1 = openFileR_vsdlss(2);
  uVar1 = tagReadIVector_vsdlss(param_1,uVar1,local_c);
  casiFClose_vsdlss(2);
  return uVar1;
}


