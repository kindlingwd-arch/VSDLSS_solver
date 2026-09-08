/* VSDLSS function (decompiled by Ghidra) */
/* name: adjustForIsolatedVars_vsdlss  addr: 009dced0  size: 43 */
#include "vsdlss_ref.h"

/* signature: void adjustForIsolatedVars_vsdlss(long param_1,long param_2); */

void adjustForIsolatedVars_vsdlss(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar2 = *(long *)(param_1 + 0x38);
  if (0 < *(int *)(param_1 + 4)) {
    lVar1 = (ulong)(*(int *)(param_1 + 4) - 1) * 8 + 8;
    lVar3 = 0;
    do {
      while (*(double *)(lVar2 + 8 + lVar3) <= 0.0) {
        *(undefined8 *)(param_2 + 8 + lVar3) = 0;
        lVar3 = lVar3 + 8;
        if (lVar3 == lVar1) {
          return;
        }
      }
      lVar3 = lVar3 + 8;
    } while (lVar3 != lVar1);
  }
  return;
}


