/* VSDLSS function (decompiled by Ghidra) */
/* name: finishChunkRead_vsdlss  addr: 009e31c0  size: 9b */
#include "vsdlss_ref.h"

/* signature: void finishChunkRead_vsdlss(void); */

void finishChunkRead_vsdlss(void)

{
  cmuChunkSizeRead = 0;
  cmuCurrentEntryRead = 0;
  cmuNumInCurrentChunk = 0xffffffff;
  casiFClose_vsdlss(cmuChunkedReadFNum);
  cmuChunkedReadFNum = 0;
  cmuChunkedReadFp = 0;
  if (cmuFromNodeVecRead != 0) {
    casiFree_vsdlss();
  }
  cmuFromNodeVecRead = 0;
  if (cmuToNodeVecRead != 0) {
    casiFree_vsdlss();
  }
  cmuToNodeVecRead = 0;
  if (cmuOffDiagVecRead != 0) {
    casiFree_vsdlss();
  }
  cmuOffDiagVecRead = 0;
  return;
}


