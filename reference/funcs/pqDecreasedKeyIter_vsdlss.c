/* VSDLSS function (decompiled by Ghidra) */
/* name: pqDecreasedKeyIter_vsdlss  addr: 00a2c830  size: 5e */
#include "vsdlss_ref.h"

/* signature: void pqDecreasedKeyIter_vsdlss(int param_1,long param_2); */

void pqDecreasedKeyIter_vsdlss(int param_1,long param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  
  lVar4 = *(long *)(param_2 + 0x20);
  lVar5 = *(long *)(param_2 + 0x18);
  lVar6 = *(long *)(param_2 + 0x10);
  while( true ) {
    if (param_1 < 2) {
      return;
    }
    iVar7 = param_1 >> 1;
    piVar1 = (int *)(lVar5 + (long)param_1 * 4);
    piVar2 = (int *)(lVar5 + (long)iVar7 * 4);
    iVar3 = *piVar1;
    if ((*(int *)(lVar4 + (long)*piVar2 * 4) < *(int *)(lVar4 + (long)iVar3 * 4)) &&
       (iVar7 != param_1)) break;
    *piVar1 = *piVar2;
    *piVar2 = iVar3;
    *(int *)(lVar6 + (long)*piVar1 * 4) = param_1;
    *(int *)(lVar6 + (long)*piVar2 * 4) = iVar7;
    param_1 = iVar7;
  }
  return;
}


