/* VSDLSS function (decompiled by Ghidra) */
/* name: casiMalloc_vsdlss  addr: 009dd400  size: 24 */
#include "vsdlss_ref.h"

/* signature: undefined8 casiMalloc_vsdlss(void); */

undefined8 casiMalloc_vsdlss(void)

{
  undefined8 uVar1;
  
  uVar1 = encapsulatedMalloc_vsdlss();
  DAT_00e965a0 = newCharPtrList_vsdlss(uVar1,DAT_00e965a0);
  return uVar1;
}


