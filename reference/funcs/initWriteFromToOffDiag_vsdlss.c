/* VSDLSS function (decompiled by Ghidra) */
/* name: initWriteFromToOffDiag_vsdlss  addr: 009f3c30  size: c2 */
#include "vsdlss_ref.h"

/* signature: void initWriteFromToOffDiag_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3); */

void initWriteFromToOffDiag_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  ftrwWriteFNum._4_4_ = param_1;
  ftrwWriteFNum._8_4_ = param_2;
  ftrwWriteFNum._12_4_ = param_3;
  ftrwWriteFp._8_8_ = openFileW_vsdlss();
  ftrwWriteFp._16_8_ = openFileW_vsdlss(ftrwWriteFNum._8_4_);
  ftrwWriteFp._24_8_ = openFileW_vsdlss(ftrwWriteFNum._12_4_);
  ftrwCurrentEntryWrite = 1;
  ftrwNumItemsWritten = 0;
  ftrwChunkSizeWrite = (int)DAT_00eb1aa8;
  ftrwFromNodeVecWrite = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeWrite,0,0);
  *ftrwFromNodeVecWrite = 0;
  ftrwToNodeVecWrite = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeWrite,0,0);
  *ftrwToNodeVecWrite = 0;
  ftrwOffDiagVecWrite = (undefined8 *)newFVector_vsdlss(0,(long)ftrwChunkSizeWrite,0);
  *ftrwOffDiagVecWrite = 0;
  return;
}


