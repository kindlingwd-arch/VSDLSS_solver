/* VSDLSS function (decompiled by Ghidra) */
/* name: initReadFromTo_vsdlss  addr: 009f41b0  size: 7e */
#include "vsdlss_ref.h"

/* signature: void initReadFromTo_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3); */

void initReadFromTo_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  ftrwReadFNum._4_4_ = param_1;
  ftrwReadFNum._8_4_ = param_2;
  ftrwReadFp._8_8_ = openFileR_vsdlss();
  ftrwReadFp._16_8_ = openFileR_vsdlss(ftrwReadFNum._8_4_);
  ftrwChunkSizeRead = (int)DAT_00eb1aa8;
  ftrwNumItemsLeft = param_3;
  ftrwCurrentEntryRead = ftrwChunkSizeRead;
  ftrwFromNodeVecRead = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeRead,0,0);
  *ftrwFromNodeVecRead = 0;
  ftrwToNodeVecRead = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeRead,0,0);
  *ftrwToNodeVecRead = 0;
  return;
}


