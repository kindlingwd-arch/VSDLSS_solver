/* VSDLSS function (decompiled by Ghidra) */
/* name: read_DEG3PARTRESNoNewVec_vsdlss  addr: 00a0e2f0  size: 29 */
#include "vsdlss_ref.h"

/* signature: undefined4 read_DEG3PARTRESNoNewVec_vsdlss(undefined8 param_1); */

undefined4 read_DEG3PARTRESNoNewVec_vsdlss(undefined8 param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  uVar2 = openFileR_vsdlss(0x13);
  uVar1 = readDVectorNoNewVec_vsdlss(uVar2,param_1);
  casiFClose_vsdlss(0x13);
  return uVar1;
}


