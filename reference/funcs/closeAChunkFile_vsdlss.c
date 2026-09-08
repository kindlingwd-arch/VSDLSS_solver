/* VSDLSS function (decompiled by Ghidra) */
/* name: closeAChunkFile_vsdlss  addr: 009e2a90  size: 31 */
#include "vsdlss_ref.h"

/* signature: void closeAChunkFile_vsdlss(long param_1,int param_2); */

void closeAChunkFile_vsdlss(long param_1,int param_2)

{
  FILE *__stream;
  
  __stream = *(FILE **)(*(long *)(param_1 + 0x30) + (long)param_2 * 8);
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
    *(undefined8 *)(*(long *)(param_1 + 0x30) + (long)param_2 * 8) = 0;
  }
  return;
}


