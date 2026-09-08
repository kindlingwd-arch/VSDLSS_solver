/* VSDLSS function (decompiled by Ghidra) */
/* name: initCasiMallocFree_vsdlss  addr: 009dd310  size: 42 */
#include "vsdlss_ref.h"

/* signature: void initCasiMallocFree_vsdlss(ulong param_1); */

void initCasiMallocFree_vsdlss(ulong param_1)

{
  DAT_00e965a0 = 0;
  DAT_00e3be20 = createTag_vsdlss();
  DAT_00e965b0 = 0;
  DAT_00e965a8 = 1;
  if (0x1f < param_1) {
    DAT_00e965a8 = param_1 >> 4;
  }
  return;
}


