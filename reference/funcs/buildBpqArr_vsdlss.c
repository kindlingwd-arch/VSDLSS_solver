/* VSDLSS function (decompiled by Ghidra) */
/* name: buildBpqArr_vsdlss  addr: 00a1f010  size: 4f */
#include "vsdlss_ref.h"

/* signature: void buildBpqArr_vsdlss(int *param_1,undefined8 param_2); */

void buildBpqArr_vsdlss(int *param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar2 = 1;
  iVar1 = *param_1;
  piVar4 = *(int **)(param_1 + 2);
  if (0 < iVar1) {
    do {
      piVar4 = piVar4 + 1;
      iVar3 = iVar2 + 1;
      nqBpq_vsdlss(iVar2,*piVar4 + 1,param_2);
      iVar2 = iVar3;
    } while (iVar3 <= iVar1);
  }
  return;
}


