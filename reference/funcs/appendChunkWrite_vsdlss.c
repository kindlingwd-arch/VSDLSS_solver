/* VSDLSS function (decompiled by Ghidra) */
/* name: appendChunkWrite_vsdlss  addr: 009e2f50  size: 61 */
#include "vsdlss_ref.h"

/* signature: void appendChunkWrite_vsdlss(undefined8 param_1,undefined4 param_2,undefined4 param_3); */

void appendChunkWrite_vsdlss(undefined8 param_1,undefined4 param_2,undefined4 param_3)

{
  *(undefined4 *)(cmuFromNodeVecWrite + (long)cmuCurrentEntryWrite * 4) = param_2;
  *(undefined4 *)(cmuToNodeVecWrite + (long)cmuCurrentEntryWrite * 4) = param_3;
  *(undefined8 *)(cmuOffDiagVecWrite + (long)cmuCurrentEntryWrite * 8) = param_1;
  cmuCurrentEntryWrite = cmuCurrentEntryWrite + 1;
  if (cmuCurrentEntryWrite <= cmuChunkSizeWrite) {
    return;
  }
  writeChunkWrite_vsdlss();
  cmuCurrentEntryWrite = 1;
  return;
}


