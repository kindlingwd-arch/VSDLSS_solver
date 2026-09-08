/* VSDLSS function (decompiled by Ghidra) */
/* name: finishReadFromTo_vsdlss  addr: 009f42f0  size: 9f */
#include "vsdlss_ref.h"

/* signature: void finishReadFromTo_vsdlss(void); */

void finishReadFromTo_vsdlss(void)

{
  casiFClose_vsdlss(ftrwReadFNum._4_4_);
  ftrwReadFp._8_8_ = 0;
  ftrwReadFNum._4_4_ = 0;
  casiFClose_vsdlss(ftrwReadFNum._8_4_);
  ftrwReadFp._16_8_ = 0;
  ftrwReadFNum._8_4_ = 0;
  ftrwChunkSizeRead = 0;
  ftrwCurrentEntryRead = 0;
  ftrwNumItemsLeft = 0;
  if (ftrwFromNodeVecRead != 0) {
    casiFree_vsdlss();
  }
  ftrwFromNodeVecRead = 0;
  if (ftrwToNodeVecRead != 0) {
    casiFree_vsdlss();
  }
  ftrwToNodeVecRead = 0;
  return;
}


