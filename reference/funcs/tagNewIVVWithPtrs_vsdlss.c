/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewIVVWithPtrs_vsdlss  addr: 00a08210  size: 24 */
#include "vsdlss_ref.h"

/* signature: void tagNewIVVWithPtrs_vsdlss(undefined8 param_1,undefined8 param_2,undefined8 param_3); */

void tagNewIVVWithPtrs_vsdlss(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)newBlock_vsdlss(param_1,0x10);
  *puVar1 = param_2;
  puVar1[1] = param_3;
  return;
}


