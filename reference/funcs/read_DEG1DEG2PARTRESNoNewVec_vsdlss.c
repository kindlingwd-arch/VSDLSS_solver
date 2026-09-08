/* VSDLSS function (decompiled by Ghidra) */
/* name: read_DEG1DEG2PARTRESNoNewVec_vsdlss  addr: 00a0e280  size: 29 */
#include "vsdlss_ref.h"

/* signature: undefined4 read_DEG1DEG2PARTRESNoNewVec_vsdlss(undefined8 param_1); */

undefined4 read_DEG1DEG2PARTRESNoNewVec_vsdlss(undefined8 param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  uVar2 = openFileR_vsdlss(0x12);
  uVar1 = readDVectorNoNewVec_vsdlss(uVar2,param_1);
  casiFClose_vsdlss(0x12);
  return uVar1;
}


