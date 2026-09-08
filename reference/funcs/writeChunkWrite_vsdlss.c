/* VSDLSS function (decompiled by Ghidra) */
/* name: writeChunkWrite_vsdlss  addr: 009e2d70  size: 133 */
#include "vsdlss_ref.h"

/* signature: void writeChunkWrite_vsdlss(int param_1); */

void writeChunkWrite_vsdlss(int param_1)

{
  int iVar1;
  size_t sVar2;
  int local_c;
  
  local_c = param_1;
  sVar2 = fwrite(&local_c,4,1,cmuChunkedWriteFp);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"chunkmatrixutils.c",0x52,"writeChunkWrite:n",1,sVar2 & 0xffffffff);
  }
  if (0 < local_c) {
    iVar1 = local_c + 1;
    sVar2 = fwrite(cmuFromNodeVecWrite,4,(long)iVar1,cmuChunkedWriteFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0x5b,"writeChunkWrite:cmuFromNodeVecWrite",iVar1,
                     sVar2 & 0xffffffff);
    }
    iVar1 = local_c + 1;
    sVar2 = fwrite(cmuToNodeVecWrite,4,(long)iVar1,cmuChunkedWriteFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0x62,"writeChunkWrite:cmuToNodeVecWrite",iVar1,
                     sVar2 & 0xffffffff);
    }
    iVar1 = local_c + 1;
    sVar2 = fwrite(cmuOffDiagVecWrite,8,(long)iVar1,cmuChunkedWriteFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0x6a,"writeChunkWrite:cmuOffDiagVecWrite",iVar1,
                     sVar2 & 0xffffffff);
    }
  }
  return;
}


