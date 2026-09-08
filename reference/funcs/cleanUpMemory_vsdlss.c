/* VSDLSS function (decompiled by Ghidra) */
/* name: cleanUpMemory_vsdlss  addr: 009dce20  size: 21 */
#include "vsdlss_ref.h"

/* signature: void cleanUpMemory_vsdlss(void); */

void cleanUpMemory_vsdlss(void)

{
  flushCasiMallocFree_vsdlss();
  deleteAllTempTags_vsdlss();
  resetTagCasiMallocFree();
  resetCelTag();
  freeMemBlasLib();
  return;
}


