/* VSDLSS function (decompiled by Ghidra) */
/* name: appendFromToOffDiag_vsdlss  addr: 009f3df0  size: 70 */
#include "vsdlss_ref.h"

/* signature: void appendFromToOffDiag_vsdlss(undefined8 param_1,undefined4 param_2,undefined4 param_3); */

void appendFromToOffDiag_vsdlss(undefined8 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  *(undefined4 *)(ftrwFromNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = param_2;
  *(undefined4 *)(ftrwToNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = param_3;
  *(undefined8 *)(ftrwOffDiagVecWrite + (long)ftrwCurrentEntryWrite * 8) = param_1;
  iVar1 = ftrwCurrentEntryWrite + 1;
  if (ftrwChunkSizeWrite < iVar1) {
    if (ftrwCurrentEntryWrite < 1) {
      ftrwCurrentEntryWrite = 1;
      return;
    }
    ftrwCurrentEntryWrite = iVar1;
    FUN_009f3b30();
    ftrwCurrentEntryWrite = 1;
    iVar1 = ftrwCurrentEntryWrite;
  }
  ftrwCurrentEntryWrite = iVar1;
  return;
}


