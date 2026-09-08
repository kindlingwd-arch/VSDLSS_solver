/* VSDLSS function (decompiled by Ghidra) */
/* name: nqPqi_vsdlss  addr: 00a0d200  size: b0 */
#include "vsdlss_ref.h"

/* signature: void nqPqi_vsdlss(int param_1,undefined4 param_2,long param_3); */

void nqPqi_vsdlss(int param_1,undefined4 param_2,long param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  int iVar11;
  long lVar12;
  
  iVar11 = *(int *)(param_3 + 8);
  if (*(int *)(param_3 + 4) <= iVar11) {
    nrerror_vsdlss(6,"priorityqutils_i.c",0x231);
    iVar11 = *(int *)(param_3 + 8);
  }
  lVar7 = *(long *)(param_3 + 0x18);
  lVar8 = *(long *)(param_3 + 0x20);
  iVar11 = iVar11 + 1;
  lVar9 = *(long *)(param_3 + 0x10);
  lVar12 = (long)iVar11;
  *(int *)(param_3 + 8) = iVar11;
  *(int *)(lVar7 + lVar12 * 4) = param_1;
  *(undefined4 *)(lVar8 + (long)param_1 * 4) = param_2;
  *(int *)(lVar9 + (long)param_1 * 4) = iVar11;
  if (1 < iVar11) {
    while( true ) {
      piVar1 = (int *)(lVar7 + lVar12 * 4);
      iVar10 = iVar11 >> 1;
      iVar3 = *piVar1;
      piVar2 = (int *)(lVar7 + (long)iVar10 * 4);
      iVar4 = *piVar2;
      iVar5 = *(int *)(lVar8 + (long)iVar4 * 4);
      iVar6 = *(int *)(lVar8 + (long)iVar3 * 4);
      if ((iVar5 <= iVar6) && (((iVar5 < iVar6 || (iVar4 < iVar3)) && (iVar10 != iVar11)))) break;
      *piVar1 = iVar4;
      *piVar2 = iVar3;
      *(int *)(lVar9 + (long)*piVar1 * 4) = iVar11;
      *(int *)(lVar9 + (long)*piVar2 * 4) = iVar10;
      if (iVar10 < 2) {
        return;
      }
      lVar12 = (long)iVar10;
      iVar11 = iVar10;
    }
  }
  return;
}


