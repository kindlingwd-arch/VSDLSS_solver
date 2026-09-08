/* VSDLSS function (decompiled by Ghidra) */
/* name: getNumDecimalDigits_vsdlss  addr: 009d9800  size: 3e */
#include "vsdlss_ref.h"

/* signature: int getNumDecimalDigits_vsdlss(uint param_1); */

int getNumDecimalDigits_vsdlss(uint param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 1;
  iVar2 = (param_1 ^ (int)param_1 >> 0x1f) - ((int)param_1 >> 0x1f);
  if (iVar2 != 0) {
    while (iVar2 = iVar2 / 10, iVar2 != 0) {
      iVar1 = iVar1 + 1;
    }
  }
  return iVar1;
}


