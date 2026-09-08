/* VSDLSS function (decompiled by Ghidra) */
/* name: finishWriteFromToOffDiag_vsdlss  addr: 009f3d20  size: c6 */
#include "vsdlss_ref.h"

/* signature: undefined4 finishWriteFromToOffDiag_vsdlss(void); */

undefined4 finishWriteFromToOffDiag_vsdlss(void)

{
  undefined4 uVar1;
  long lVar2;
  
  if (0 < ftrwCurrentEntryWrite + -1) {
    FUN_009f3b30();
  }
  uVar1 = ftrwNumItemsWritten;
  lVar2 = 0;
  do {
    casiFClose_vsdlss(*(undefined4 *)(ftrwWriteFNum + lVar2 + 4));
    *(undefined4 *)(ftrwWriteFNum + lVar2 + 4) = 0;
    *(undefined8 *)(ftrwWriteFp + lVar2 * 2 + 8) = 0;
    lVar2 = lVar2 + 4;
  } while (lVar2 != 0xc);
  ftrwChunkSizeWrite = 0;
  ftrwCurrentEntryWrite = 0;
  ftrwNumItemsWritten = 0;
  if (ftrwFromNodeVecWrite != 0) {
    casiFree_vsdlss();
  }
  ftrwFromNodeVecWrite = 0;
  if (ftrwToNodeVecWrite != 0) {
    casiFree_vsdlss();
  }
  ftrwToNodeVecWrite = 0;
  if (ftrwOffDiagVecWrite != 0) {
    casiFree_vsdlss();
  }
  ftrwOffDiagVecWrite = 0;
  return uVar1;
}


