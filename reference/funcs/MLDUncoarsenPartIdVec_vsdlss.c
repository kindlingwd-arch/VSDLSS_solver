/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDUncoarsenPartIdVec_vsdlss  addr: 00a00bf0  size: 74 */
#include "vsdlss_ref.h"

/* signature: void MLDUncoarsenPartIdVec_vsdlss(int param_1,long param_2,long param_3); */

void MLDUncoarsenPartIdVec_vsdlss(int param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  ulong uVar5;
  
  lVar1 = *(long *)(param_3 + 0xa0);
  lVar2 = *(long *)(param_3 + 0x60);
  lVar3 = *(long *)(param_3 + 0xa8);
  if (0 < param_1) {
    lVar4 = 0;
    do {
      *(undefined4 *)(lVar2 + 4 + lVar4) =
           *(undefined4 *)(lVar1 + (long)*(int *)(param_2 + 4 + lVar4) * 4);
      *(undefined4 *)(param_2 + 4 + lVar4) =
           *(undefined4 *)(lVar3 + (long)*(int *)(param_2 + 4 + lVar4) * 4);
      lVar4 = lVar4 + 4;
    } while (lVar4 != (ulong)(param_1 - 1) * 4 + 4);
  }
  uVar5 = 1;
  if ((long)param_1 == 0) {
    return;
  }
  do {
    *(undefined4 *)(lVar1 + uVar5 * 4) = *(undefined4 *)(lVar2 + uVar5 * 4);
    uVar5 = uVar5 + 1;
  } while (uVar5 <= (ulong)(long)param_1);
  return;
}


