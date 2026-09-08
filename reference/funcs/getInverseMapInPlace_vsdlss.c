/* VSDLSS function (decompiled by Ghidra) */
/* name: getInverseMapInPlace_vsdlss  addr: 009d8bb0  size: 76 */
#include "vsdlss_ref.h"

/* signature: void getInverseMapInPlace_vsdlss(int *param_1,int param_2); */

void getInverseMapInPlace_vsdlss(int *param_1,int param_2)

{
  long lVar1;
  int *piVar2;
  int iVar3;
  ulong uVar4;
  ulong uVar5;
  
  uVar5 = (ulong)param_2;
  lVar1 = newIVector_vsdlss(uVar5,0,0);
  iVar3 = 1;
  piVar2 = param_1;
  if (0 < param_2) {
    do {
      piVar2 = piVar2 + 1;
      *(int *)(lVar1 + (long)*piVar2 * 4) = iVar3;
      iVar3 = iVar3 + 1;
    } while (iVar3 != param_2 + 1);
  }
  uVar4 = 1;
  if (uVar5 != 0) {
    do {
      param_1[uVar4] = *(int *)(lVar1 + uVar4 * 4);
      uVar4 = uVar4 + 1;
    } while (uVar4 <= uVar5);
  }
  if (lVar1 == 0) {
    return;
  }
  casiFree_vsdlss(lVar1);
  return;
}


