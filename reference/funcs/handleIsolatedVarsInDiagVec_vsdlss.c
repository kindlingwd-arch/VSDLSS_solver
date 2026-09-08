/* VSDLSS function (decompiled by Ghidra) */
/* name: handleIsolatedVarsInDiagVec_vsdlss  addr: 009dcf20  size: 3f */
#include "vsdlss_ref.h"

/* signature: void handleIsolatedVarsInDiagVec_vsdlss(int param_1,long param_2,int param_3); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void handleIsolatedVarsInDiagVec_vsdlss(int param_1,long param_2,int param_3)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  
  if (0 < param_1 - param_3) {
    pdVar1 = (double *)(param_2 + 0x10 + (ulong)((param_1 - param_3) - 1) * 8);
    pdVar2 = (double *)(param_2 + 8);
    do {
      while (*pdVar2 <= 0.0) {
        pdVar3 = pdVar2 + 1;
        *pdVar2 = _DAT_00ab67f8;
        pdVar2 = pdVar3;
        if (pdVar3 == pdVar1) {
          return;
        }
      }
      pdVar2 = pdVar2 + 1;
    } while (pdVar2 != pdVar1);
  }
  return;
}


