/* VSDLSS function (decompiled by Ghidra) */
/* name: newIVVWithPtrs_vsdlss  addr: 00a081e0  size: 24 */
#include "vsdlss_ref.h"

/* signature: void newIVVWithPtrs_vsdlss(undefined8 param_1,undefined8 param_2); */

void newIVVWithPtrs_vsdlss(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)casiMalloc_vsdlss(0x10);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  return;
}


