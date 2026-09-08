/* VSDLSS function (decompiled by Ghidra) */
/* name: getNextChunkRead_vsdlss  addr: 009e3260  size: e4 */
#include "vsdlss_ref.h"

/* signature: undefined8 getNextChunkRead_vsdlss(undefined4 *param_1,undefined4 *param_2,undefined8 *param_3); */

undefined8 getNextChunkRead_vsdlss(undefined4 *param_1,undefined4 *param_2,undefined8 *param_3)

{
  cmuCurrentEntryRead = cmuCurrentEntryRead + 1;
  if (cmuChunkSizeRead < cmuCurrentEntryRead) {
    cmuCurrentEntryRead = 1;
    cmuNumInCurrentChunk = readChunkRead_vsdlss();
    if (cmuChunkSizeRead < cmuNumInCurrentChunk) {
      nrerror_vsdlss(1,"chunkmatrixutils.c",0x113);
    }
    if (cmuNumInCurrentChunk == 0) goto LAB_009e32f1;
  }
  else if (cmuNumInCurrentChunk < cmuCurrentEntryRead) {
LAB_009e32f1:
    *param_1 = 0xffffffff;
    *param_2 = 0xffffffff;
    cmuCurrentEntryRead = cmuNumInCurrentChunk;
    *param_3 = 0;
    return 1;
  }
  *param_1 = *(undefined4 *)(cmuFromNodeVecRead + (long)cmuCurrentEntryRead * 4);
  *param_2 = *(undefined4 *)(cmuToNodeVecRead + (long)cmuCurrentEntryRead * 4);
  *param_3 = *(undefined8 *)(cmuOffDiagVecRead + (long)cmuCurrentEntryRead * 8);
  return 0;
}


