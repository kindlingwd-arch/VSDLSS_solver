/* VSDLSS function (decompiled by Ghidra) */
/* name: readChunkRead_vsdlss  addr: 009e3080  size: 136 */
#include "vsdlss_ref.h"

/* signature: int readChunkRead_vsdlss(void); */

int readChunkRead_vsdlss(void)

{
  int iVar1;
  size_t sVar2;
  int local_c;
  
  sVar2 = fread(&local_c,4,1,cmuChunkedReadFp);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"chunkmatrixutils.c",0xcc,"readChunkRead:n",1,sVar2 & 0xffffffff);
  }
  if (0 < local_c) {
    iVar1 = local_c + 1;
    sVar2 = fread(cmuFromNodeVecRead,4,(long)iVar1,cmuChunkedReadFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0xd1,"readChunkRead:cmuFromNodeVecRead",iVar1,
                     sVar2 & 0xffffffff);
    }
    iVar1 = local_c + 1;
    sVar2 = fread(cmuToNodeVecRead,4,(long)iVar1,cmuChunkedReadFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0xd5,"readChunkRead:cmuToNodeVecRead",iVar1,
                     sVar2 & 0xffffffff);
    }
    iVar1 = local_c + 1;
    sVar2 = fread(cmuOffDiagVecRead,8,(long)iVar1,cmuChunkedReadFp);
    if ((int)sVar2 != iVar1) {
      nrerror_vsdlss(0x10,"chunkmatrixutils.c",0xdd,"readChunkRead:cmuOffDiagVecRead",iVar1,
                     sVar2 & 0xffffffff);
    }
  }
  return local_c;
}


