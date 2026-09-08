/* VSDLSS function (decompiled by Ghidra) */
/* name: newMLDGraphVector_vsdlss  addr: 009fc440  size: 7a */
#include "vsdlss_ref.h"

/* signature: long newMLDGraphVector_vsdlss(int param_1,int param_2,undefined8 param_3); */

long newMLDGraphVector_vsdlss(int param_1,int param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  ulong uVar4;
  
  uVar4 = (ulong)param_1;
  lVar1 = uVar4 * 8 + 8;
  lVar2 = casiMalloc_vsdlss(lVar1);
  if (lVar2 == 0) {
    nrerror_vsdlss(0xd,"mldorder.c",0x137,lVar1,"polyNewVector()");
  }
  if ((param_2 != 0) && (uVar4 != 0)) {
    uVar3 = 1;
    do {
      *(undefined8 *)(lVar2 + uVar3 * 8) = param_3;
      uVar3 = uVar3 + 1;
    } while (uVar3 <= uVar4);
  }
  return lVar2;
}


