/* VSDLSS function (decompiled by Ghidra) */
/* name: handleIsolatedVarsInEa_vsdlss  addr: 009dcf70  size: 44 */
#include "vsdlss_ref.h"

/* signature: void handleIsolatedVarsInEa_vsdlss(long param_1); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void handleIsolatedVarsInEa_vsdlss(long param_1)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  
  if (0 < *(int *)(param_1 + 4)) {
    pdVar1 = (double *)(*(long *)(param_1 + 0x38) + 0x10 + (ulong)(*(int *)(param_1 + 4) - 1) * 8);
    pdVar2 = (double *)(*(long *)(param_1 + 0x38) + 8);
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


