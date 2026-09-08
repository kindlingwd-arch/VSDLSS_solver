/* VSDLSS function (decompiled by Ghidra) */
/* name: getNextFromToOffDiag_vsdlss  addr: 009f40e0  size: c0 */
#include "vsdlss_ref.h"

/* signature: undefined8 getNextFromToOffDiag_vsdlss(undefined4 *param_1,undefined4 *param_2,undefined8 *param_3); */

undefined8 getNextFromToOffDiag_vsdlss(undefined4 *param_1,undefined4 *param_2,undefined8 *param_3)

{
  long lVar1;
  
  if (0 < ftrwNumItemsLeft) {
    ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
    if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
      readFromToOffDiag_vsdlss();
      ftrwCurrentEntryRead = 1;
      lVar1 = 4;
    }
    else {
      lVar1 = (long)ftrwCurrentEntryRead << 2;
    }
    *param_1 = *(undefined4 *)(ftrwFromNodeVecRead + lVar1);
    *param_2 = *(undefined4 *)(ftrwToNodeVecRead + (long)ftrwCurrentEntryRead * 4);
    *param_3 = *(undefined8 *)(ftrwOffDiagVecRead + (long)ftrwCurrentEntryRead * 8);
    ftrwNumItemsLeft = ftrwNumItemsLeft + -1;
    return 0;
  }
  *param_1 = 0xffffffff;
  *param_2 = 0xffffffff;
  *param_3 = 0;
  return 1;
}


