/* VSDLSS function (decompiled by Ghidra) */
/* name: buildBpq_vsdlss  addr: 00a1f060  size: 55 */
#include "vsdlss_ref.h"

/* signature: void buildBpq_vsdlss(int *param_1,undefined8 param_2); */

void buildBpq_vsdlss(int *param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  
  iVar3 = 1;
  iVar1 = *param_1;
  puVar5 = *(undefined8 **)(param_1 + 6);
  if (0 < iVar1) {
    do {
      puVar5 = puVar5 + 1;
      iVar2 = getCEdgelistSize_vsdlss(*puVar5);
      iVar4 = iVar3 + 1;
      nqBpq_vsdlss(iVar3,iVar2 + 1,param_2);
      iVar3 = iVar4;
    } while (iVar4 <= iVar1);
  }
  return;
}


