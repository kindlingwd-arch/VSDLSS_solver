/* VSDLSS function (decompiled by Ghidra) */
/* name: getRowToBlockVec_vsdlss  addr: 00a1b0b0  size: 5d */
#include "vsdlss_ref.h"

/* signature: void getRowToBlockVec_vsdlss(int param_1,int *param_2,long param_3); */

void getRowToBlockVec_vsdlss(int param_1,int *param_2,long param_3)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  if (0 < param_1) {
    iVar4 = 0;
    iVar3 = 1;
    do {
      param_2 = param_2 + 1;
      iVar1 = *param_2;
      if (0 < iVar1) {
        piVar2 = (int *)(param_3 + 4 + (long)iVar4 * 4);
        do {
          *piVar2 = iVar3;
          piVar2 = piVar2 + 1;
        } while (piVar2 != (int *)(param_3 + 8 + ((long)iVar4 + (ulong)(iVar1 - 1)) * 4));
        iVar4 = iVar4 + iVar1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 != param_1 + 1);
  }
  return;
}


