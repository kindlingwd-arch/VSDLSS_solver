/* VSDLSS function (decompiled by Ghidra) */
/* name: write_DEG3PARTRES_vsdlss  addr: 00a0e2b0  size: 32 */
#include "vsdlss_ref.h"

/* signature: void write_DEG3PARTRES_vsdlss(undefined8 param_1,undefined4 param_2); */

void write_DEG3PARTRES_vsdlss(undefined8 param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  uVar1 = openFileW_vsdlss(0x13);
  writeDVector_vsdlss(param_1,param_2,uVar1);
  casiFClose_vsdlss(0x13);
  return;
}


