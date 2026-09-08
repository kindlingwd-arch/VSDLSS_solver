/* VSDLSS function (decompiled by Ghidra) */
/* name: read_DEG123REORDERVEC_vsdlss  addr: 00a0e3e0  size: 32 */
#include "vsdlss_ref.h"

/* signature: undefined8 read_DEG123REORDERVEC_vsdlss(void); */

undefined8 read_DEG123REORDERVEC_vsdlss(void)

{
  undefined8 uVar1;
  undefined1 local_c [4];
  
  uVar1 = openFileR_vsdlss(0x10);
  uVar1 = readIVector_vsdlss(uVar1,local_c);
  casiFClose_vsdlss(0x10);
  return uVar1;
}


