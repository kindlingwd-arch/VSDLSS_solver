/* VSDLSS function (decompiled by Ghidra) */
/* name: finishChunkWrite_vsdlss  addr: 009e2eb0  size: 9f */
#include "vsdlss_ref.h"

/* signature: void finishChunkWrite_vsdlss(void); */

void finishChunkWrite_vsdlss(void)

{
  writeChunkWrite_vsdlss(cmuCurrentEntryWrite + -1);
  cmuChunkSizeWrite = 0;
  cmuCurrentEntryWrite = 0;
  casiFClose_vsdlss(cmuChunkedWriteFNum);
  cmuChunkedWriteFNum = 0;
  cmuChunkedWriteFp = 0;
  if (cmuFromNodeVecWrite != 0) {
    casiFree_vsdlss();
  }
  cmuFromNodeVecWrite = 0;
  if (cmuToNodeVecWrite != 0) {
    casiFree_vsdlss();
  }
  cmuToNodeVecWrite = 0;
  if (cmuOffDiagVecWrite != 0) {
    casiFree_vsdlss();
  }
  cmuOffDiagVecWrite = 0;
  return;
}


