/* VSDLSS function (decompiled by Ghidra) */
/* name: finishReadFromToOffDiag_vsdlss  addr: 009f4030  size: a2 */
#include "vsdlss_ref.h"

/* signature: void finishReadFromToOffDiag_vsdlss(void); */

void finishReadFromToOffDiag_vsdlss(void)

{
  long lVar1;
  
  lVar1 = 0;
  do {
    casiFClose_vsdlss(*(undefined4 *)(ftrwReadFNum + lVar1 + 4));
    *(undefined4 *)(ftrwReadFNum + lVar1 + 4) = 0;
    *(undefined8 *)(ftrwReadFp + lVar1 * 2 + 8) = 0;
    lVar1 = lVar1 + 4;
  } while (lVar1 != 0xc);
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
  if (ftrwOffDiagVecRead != 0) {
    casiFree_vsdlss();
  }
  ftrwOffDiagVecRead = 0;
  return;
}


