/* VSDLSS function (decompiled by Ghidra) */
/* name: newCPVector_vsdlss  addr: 009d8170  size: 7a */
#include "vsdlss_ref.h"

/* signature: long newCPVector_vsdlss(ulong param_1,int param_2,undefined8 param_3); */

long newCPVector_vsdlss(ulong param_1,int param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  lVar1 = param_1 * 8 + 8;
  lVar2 = casiMalloc_vsdlss(lVar1);
  if (lVar2 == 0) {
    nrerror_vsdlss(0xd,"utils.c",0x2c9,lVar1,"polyNewVector()");
  }
  if ((param_2 != 0) && (param_1 != 0)) {
    uVar3 = 1;
    do {
      *(undefined8 *)(lVar2 + uVar3 * 8) = param_3;
      uVar3 = uVar3 + 1;
    } while (uVar3 <= param_1);
  }
  return lVar2;
}


