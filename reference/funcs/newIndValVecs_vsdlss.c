/* VSDLSS function (decompiled by Ghidra) */
/* name: newIndValVecs_vsdlss  addr: 00a08240  size: 41 */
#include "vsdlss_ref.h"

/* signature: undefined8 * newIndValVecs_vsdlss(int param_1); */

undefined8 * newIndValVecs_vsdlss(int param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)casiMalloc_vsdlss(0x10);
  uVar2 = newIVector_vsdlss((long)param_1,0,0);
  *puVar1 = uVar2;
  uVar2 = newVector_vsdlss(0,(long)param_1,0);
  puVar1[1] = uVar2;
  return puVar1;
}


