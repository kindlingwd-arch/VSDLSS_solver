/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDConvertCoverToSeparator_vsdlss  addr: 00a01cc0  size: 29 */
#include "vsdlss_ref.h"

/* signature: void MLDConvertCoverToSeparator_vsdlss(long param_1,long param_2,int param_3,long param_4); */

void MLDConvertCoverToSeparator_vsdlss(long param_1,long param_2,int param_3,long param_4)

{
  int iVar1;
  int *piVar2;
  
  if (0 < param_3) {
    piVar2 = (int *)(param_4 + 4);
    do {
      iVar1 = *piVar2;
      piVar2 = piVar2 + 1;
      *(undefined4 *)(param_1 + (long)*(int *)(param_2 + (long)iVar1 * 4) * 4) = 3;
    } while (piVar2 != (int *)(param_4 + 8 + (ulong)(param_3 - 1) * 4));
  }
  return;
}


