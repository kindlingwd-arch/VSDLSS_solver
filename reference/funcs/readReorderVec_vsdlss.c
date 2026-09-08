/* VSDLSS function (decompiled by Ghidra) */
/* name: readReorderVec_vsdlss  addr: 00a0e520  size: 32 */
#include "vsdlss_ref.h"

/* signature: undefined8 readReorderVec_vsdlss(void); */

undefined8 readReorderVec_vsdlss(void)

{
  undefined8 uVar1;
  undefined1 local_c [4];
  
  uVar1 = openFileR_vsdlss(2);
  uVar1 = readIVector_vsdlss(uVar1,local_c);
  casiFClose_vsdlss(2);
  return uVar1;
}


