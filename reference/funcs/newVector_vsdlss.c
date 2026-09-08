/* VSDLSS function (decompiled by Ghidra) */
/* name: newVector_vsdlss  addr: 009d8060  size: 8b */
#include "vsdlss_ref.h"

/* signature: long newVector_vsdlss(undefined8 param_1,ulong param_2,int param_3); */

long newVector_vsdlss(undefined8 param_1,ulong param_2,int param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  lVar1 = param_2 * 8 + 8;
  lVar2 = casiMalloc_vsdlss(lVar1);
  if (lVar2 == 0) {
    nrerror_vsdlss(0xd,"utils.c",0x2a1,lVar1,"polyNewVector()");
  }
  if ((param_3 != 0) && (param_2 != 0)) {
    uVar3 = 1;
    do {
      *(undefined8 *)(lVar2 + uVar3 * 8) = param_1;
      uVar3 = uVar3 + 1;
    } while (uVar3 <= param_2);
  }
  return lVar2;
}


