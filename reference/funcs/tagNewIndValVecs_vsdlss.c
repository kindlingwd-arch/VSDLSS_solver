/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewIndValVecs_vsdlss  addr: 00a08290  size: 43 */
#include "vsdlss_ref.h"

/* signature: undefined8 * tagNewIndValVecs_vsdlss(ulong param_1,int param_2); */

undefined8 * tagNewIndValVecs_vsdlss(ulong param_1,int param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)newBlock_vsdlss(param_1,0x10);
  uVar2 = tagNewIVector_vsdlss(param_1 & 0xffffffff,(long)param_2,0,0);
  *puVar1 = uVar2;
  uVar2 = tagNewFVector_vsdlss(0,param_1 & 0xffffffff,(long)param_2,0);
  puVar1[1] = uVar2;
  return puVar1;
}


