/* VSDLSS function (decompiled by Ghidra) */
/* name: getNextFromTo_vsdlss  addr: 009f4390  size: 96 */
#include "vsdlss_ref.h"

/* signature: undefined8 getNextFromTo_vsdlss(undefined4 *param_1,undefined4 *param_2); */

undefined8 getNextFromTo_vsdlss(undefined4 *param_1,undefined4 *param_2)

{
  long lVar1;
  
  if (0 < ftrwNumItemsLeft) {
    ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
    if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
      readFromTo();
      ftrwCurrentEntryRead = 1;
      lVar1 = 4;
    }
    else {
      lVar1 = (long)ftrwCurrentEntryRead << 2;
    }
    *param_1 = *(undefined4 *)(ftrwFromNodeVecRead + lVar1);
    *param_2 = *(undefined4 *)(ftrwToNodeVecRead + (long)ftrwCurrentEntryRead * 4);
    ftrwNumItemsLeft = ftrwNumItemsLeft + -1;
    return 0;
  }
  *param_1 = 0xffffffff;
  *param_2 = 0xffffffff;
  return 1;
}


