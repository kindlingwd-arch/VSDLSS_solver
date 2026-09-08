/* VSDLSS function (decompiled by Ghidra) */
/* name: newCharPtrList_vsdlss  addr: 009dd360  size: 67 */
#include "vsdlss_ref.h"

/* signature: void newCharPtrList_vsdlss(undefined8 param_1,undefined8 param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void newCharPtrList_vsdlss(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  if (DAT_00e3be20 < 0) {
    uVar1 = getMemAllocBlockSize_vsdlss();
    initCasiMallocFree_vsdlss(uVar1);
  }
  _DAT_00e96598 = _DAT_00e96598 + 1;
  if (DAT_00e965b0 == (undefined8 *)0x0) {
    DAT_00e965b0 = (undefined8 *)newCharPtrChunk_vsdlss();
  }
  uVar1 = DAT_00e965b0[1];
  *DAT_00e965b0 = param_1;
  DAT_00e965b0[1] = param_2;
  DAT_00e965b0 = (undefined8 *)uVar1;
  return;
}


