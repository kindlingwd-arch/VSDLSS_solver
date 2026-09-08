/* VSDLSS function (decompiled by Ghidra) */
/* name: bucketNodesByDegree_vsdlss  addr: 009fd940  size: d3 */
#include "vsdlss_ref.h"

/* signature: void bucketNodesByDegree_vsdlss(int param_1,int param_2,long param_3,long param_4,long param_5,long param_6); */

void bucketNodesByDegree_vsdlss
               (int param_1,int param_2,long param_3,long param_4,long param_5,long param_6)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  initIVector_vsdlss(param_6,(long)param_1,0);
  if (0 < param_2) {
    piVar2 = (int *)(param_4 + 4);
    do {
      iVar4 = *piVar2 + 1;
      if (param_1 < *piVar2 + 1) {
        iVar4 = param_1;
      }
      piVar2 = piVar2 + 1;
      piVar1 = (int *)(param_6 + (long)iVar4 * 4);
      *piVar1 = *piVar1 + 1;
    } while (piVar2 != (int *)(param_4 + 8 + (ulong)(param_2 - 1) * 4));
  }
  if (0 < param_1) {
    piVar2 = (int *)(param_6 + 4);
    iVar4 = 1;
    do {
      iVar3 = *piVar2;
      *piVar2 = iVar4;
      piVar2 = piVar2 + 1;
      iVar4 = iVar4 + iVar3;
    } while (piVar2 != (int *)(param_6 + 8 + (ulong)(param_1 - 1) * 4));
  }
  if (0 < param_2) {
    piVar2 = (int *)(param_3 + 4);
    do {
      iVar4 = *piVar2;
      iVar3 = *(int *)(param_4 + (long)iVar4 * 4) + 1;
      if (param_1 < iVar3) {
        iVar3 = param_1;
      }
      piVar2 = piVar2 + 1;
      piVar1 = (int *)(param_6 + (long)iVar3 * 4);
      *(int *)(param_5 + (long)*piVar1 * 4) = iVar4;
      *piVar1 = *piVar1 + 1;
    } while (piVar2 != (int *)(param_3 + 8 + (ulong)(param_2 - 1) * 4));
  }
  return;
}


