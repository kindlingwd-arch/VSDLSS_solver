/* VSDLSS function (decompiled by Ghidra) */
/* name: flushCasiMallocFree_vsdlss  addr: 009dd510  size: 4e */
#include "vsdlss_ref.h"

/* signature: void flushCasiMallocFree_vsdlss(void); */

void flushCasiMallocFree_vsdlss(void)

{
  undefined8 *puVar1;
  
  puVar1 = DAT_00e965a0;
  while (puVar1 != (undefined8 *)0x0) {
    DAT_00e965a0 = (undefined8 *)puVar1[1];
    encapsulatedFree_vsdlss(*puVar1);
    puVar1[1] = DAT_00e965b0;
    DAT_00e965b0 = puVar1;
    puVar1 = DAT_00e965a0;
  }
  DAT_00e965a0 = (undefined8 *)0x0;
  return;
}


