/* VSDLSS function (decompiled by Ghidra) */
/* name: buildPq_vsdlss  addr: 00a2cb80  size: 5a */
#include "vsdlss_ref.h"

/* signature: void buildPq_vsdlss(long param_1,int param_2); */

void buildPq_vsdlss(long param_1,int param_2)

{
  long lVar1;
  int iVar2;
  int *piVar3;
  
  *(int *)(param_1 + 8) = param_2;
  if (0 < param_2) {
    piVar3 = *(int **)(param_1 + 0x18);
    lVar1 = *(long *)(param_1 + 0x10);
    iVar2 = 1;
    do {
      piVar3 = piVar3 + 1;
      *(int *)(lVar1 + (long)*piVar3 * 4) = iVar2;
      iVar2 = iVar2 + 1;
    } while (iVar2 != param_2 + 1);
    param_2 = *(int *)(param_1 + 8);
  }
  param_2 = param_2 / 2;
  if (0 < param_2) {
    do {
      pqIncreasedKeyIter_vsdlss(param_2,param_1);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}


