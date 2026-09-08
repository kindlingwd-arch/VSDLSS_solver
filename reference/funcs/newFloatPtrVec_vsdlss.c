/* VSDLSS function (decompiled by Ghidra) */
/* name: newFloatPtrVec_vsdlss  addr: 009d89a0  size: 51 */
#include "vsdlss_ref.h"

/* signature: long newFloatPtrVec_vsdlss(long param_1); */

long newFloatPtrVec_vsdlss(long param_1)

{
  long lVar1;
  long lVar2;
  
  lVar1 = param_1 * 8 + 8;
  lVar2 = casiMalloc_vsdlss(lVar1);
  if (lVar2 != 0) {
    return lVar2;
  }
  nrerror_vsdlss(0xd,"utils.c",0x462,lVar1,"polyNewVector()");
  return 0;
}


