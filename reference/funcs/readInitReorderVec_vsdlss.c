/* VSDLSS function (decompiled by Ghidra) */
/* name: readInitReorderVec_vsdlss  addr: 00a0e660  size: 32 */
#include "vsdlss_ref.h"

/* signature: undefined8 readInitReorderVec_vsdlss(void); */

undefined8 readInitReorderVec_vsdlss(void)

{
  undefined8 uVar1;
  undefined1 local_c [4];
  
  uVar1 = openFileR_vsdlss(7);
  uVar1 = readIVector_vsdlss(uVar1,local_c);
  casiFClose_vsdlss(7);
  return uVar1;
}


