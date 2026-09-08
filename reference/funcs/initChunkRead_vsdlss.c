/* VSDLSS function (decompiled by Ghidra) */
/* name: initChunkRead_vsdlss  addr: 009e2fc0  size: b4 */
#include "vsdlss_ref.h"

/* signature: void initChunkRead_vsdlss(undefined4 param_1); */

void initChunkRead_vsdlss(undefined4 param_1)

{
  size_t sVar1;
  
  cmuChunkedReadFNum = param_1;
  cmuChunkedReadFp = (FILE *)openFileR_vsdlss();
  sVar1 = fread(&cmuChunkSizeRead,4,1,cmuChunkedReadFp);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"chunkmatrixutils.c",0xb2,"initChunkRead:cmuChunkSizeRead",1,
                   sVar1 & 0xffffffff);
  }
  cmuFromNodeVecRead = newIVector_vsdlss((long)cmuChunkSizeRead,0,0);
  cmuToNodeVecRead = newIVector_vsdlss((long)cmuChunkSizeRead,0,0);
  cmuOffDiagVecRead = newFVector_vsdlss(0,(long)cmuChunkSizeRead,0);
  cmuCurrentEntryRead = cmuChunkSizeRead;
  cmuNumInCurrentChunk = cmuChunkSizeRead;
  return;
}


