/* VSDLSS function (decompiled by Ghidra) */
/* name: readFromToOffDiag_vsdlss  addr: 009f3f30  size: f5 */
#include "vsdlss_ref.h"

/* signature: void readFromToOffDiag_vsdlss(void); */

void readFromToOffDiag_vsdlss(void)

{
  int iVar1;
  size_t sVar2;
  size_t __n;
  
  iVar1 = ftrwChunkSizeRead;
  if (ftrwNumItemsLeft <= ftrwChunkSizeRead) {
    iVar1 = ftrwNumItemsLeft;
  }
  __n = (size_t)iVar1;
  sVar2 = fread((void *)(ftrwFromNodeVecRead + 4),4,__n,(FILE *)ftrwReadFp._8_8_);
  if (iVar1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"fromtoreadwrite.c",0xd2,"readFromToOffDiag:ftrwFromNodeVecRead",iVar1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread((void *)(ftrwToNodeVecRead + 4),4,__n,(FILE *)ftrwReadFp._16_8_);
  if (iVar1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"fromtoreadwrite.c",0xd6,"readFromToOffDiag:ftrwToNodeVecRead",iVar1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread((void *)(ftrwOffDiagVecRead + 8),8,__n,(FILE *)ftrwReadFp._24_8_);
  if (iVar1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"fromtoreadwrite.c",0xde,"readFromToOffDiag:ftrwOffDiagVecRead",iVar1,
                   sVar2 & 0xffffffff);
    return;
  }
  return;
}


