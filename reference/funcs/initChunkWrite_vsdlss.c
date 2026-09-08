/* VSDLSS function (decompiled by Ghidra) */
/* name: initChunkWrite_vsdlss  addr: 009e2c90  size: d4 */
#include "vsdlss_ref.h"

/* signature: void initChunkWrite_vsdlss(undefined4 param_1); */

void initChunkWrite_vsdlss(undefined4 param_1)

{
  undefined8 *puVar1;
  size_t sVar2;
  
  cmuChunkSizeWrite = (int)DAT_00ead8a0;
  cmuFromNodeVecWrite = (undefined4 *)newIVector_vsdlss((long)cmuChunkSizeWrite,0,0);
  cmuToNodeVecWrite = (undefined4 *)newIVector_vsdlss((long)cmuChunkSizeWrite,0,0);
  puVar1 = (undefined8 *)newFVector_vsdlss(0,(long)cmuChunkSizeWrite,0);
  cmuOffDiagVecWrite = puVar1;
  *cmuFromNodeVecWrite = 0;
  *cmuToNodeVecWrite = 0;
  *puVar1 = 0;
  cmuChunkedWriteFNum = param_1;
  cmuChunkedWriteFp = (FILE *)openFileW_vsdlss(param_1);
  sVar2 = fwrite(&cmuChunkSizeWrite,4,1,cmuChunkedWriteFp);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"chunkmatrixutils.c",0x3e,"initChunkWrite:cmuChunkSizeWrite",1,
                   sVar2 & 0xffffffff);
  }
  cmuCurrentEntryWrite = 1;
  return;
}


