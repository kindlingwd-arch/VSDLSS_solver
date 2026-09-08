/* VSDLSS function (decompiled by Ghidra) */
/* name: checkForNegativeDiags_vsdlss  addr: 009f6070  size: 6f */
#include "vsdlss_ref.h"

/* signature: void checkForNegativeDiags_vsdlss(int param_1,long param_2); */

void checkForNegativeDiags_vsdlss(int param_1,long param_2)

{
  int iVar1;
  double *pdVar2;
  
  pdVar2 = (double *)(param_2 + 8);
  iVar1 = 1;
  if (0 < param_1) {
    do {
      while (0.0 < *pdVar2 || *pdVar2 == 0.0) {
        iVar1 = iVar1 + 1;
        pdVar2 = pdVar2 + 1;
        if (param_1 < iVar1) {
          return;
        }
      }
      iVar1 = iVar1 + 1;
      pdVar2 = pdVar2 + 1;
      nrerror_vsdlss(0x15,"interface.c",0x27b);
    } while (iVar1 <= param_1);
  }
  return;
}


