/* VSDLSS function (decompiled by Ghidra) */
/* name: initReadFromToOffDiag_vsdlss  addr: 009f3e70  size: b6 */
#include "vsdlss_ref.h"

/* signature: void initReadFromToOffDiag_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4); */

void initReadFromToOffDiag_vsdlss
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  ftrwReadFNum._4_4_ = param_1;
  ftrwReadFNum._8_4_ = param_2;
  ftrwReadFNum._12_4_ = param_3;
  ftrwReadFp._8_8_ = openFileR_vsdlss();
  ftrwReadFp._16_8_ = openFileR_vsdlss(ftrwReadFNum._8_4_);
  ftrwReadFp._24_8_ = openFileR_vsdlss(ftrwReadFNum._12_4_);
  ftrwChunkSizeRead = (int)DAT_00eb1aa8;
  ftrwNumItemsLeft = param_4;
  ftrwCurrentEntryRead = ftrwChunkSizeRead;
  ftrwFromNodeVecRead = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeRead,0,0);
  *ftrwFromNodeVecRead = 0;
  ftrwToNodeVecRead = (undefined4 *)newIVector_vsdlss((long)ftrwChunkSizeRead,0,0);
  *ftrwToNodeVecRead = 0;
  ftrwOffDiagVecRead = (undefined8 *)newFVector_vsdlss(0,(long)ftrwChunkSizeRead,0);
  *ftrwOffDiagVecRead = 0;
  return;
}


