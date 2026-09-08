/* VSDLSS function (decompiled by Ghidra) */
/* name: newCVector_vsdlss  addr: 009d80f0  size: 72 */
#include "vsdlss_ref.h"

/* signature: long newCVector_vsdlss(ulong param_1,int param_2,undefined1 param_3); */

long newCVector_vsdlss(ulong param_1,int param_2,undefined1 param_3)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = casiMalloc_vsdlss(param_1 + 1);
  if (lVar1 == 0) {
    nrerror_vsdlss(0xd,"utils.c",0x2b5,param_1 + 1,"polyNewVector()");
  }
  if ((param_2 != 0) && (param_1 != 0)) {
    uVar2 = 1;
    do {
      *(undefined1 *)(lVar1 + uVar2) = param_3;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_1);
  }
  return lVar1;
}


