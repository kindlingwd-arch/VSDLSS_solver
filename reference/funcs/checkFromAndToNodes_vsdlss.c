/* VSDLSS function (decompiled by Ghidra) */
/* name: checkFromAndToNodes_vsdlss  addr: 009f5ff0  size: 76 */
#include "vsdlss_ref.h"

/* signature: void checkFromAndToNodes_vsdlss(int param_1,int param_2,long param_3,long param_4); */

void checkFromAndToNodes_vsdlss(int param_1,int param_2,long param_3,long param_4)

{
  int iVar1;
  int iVar2;
  long lVar3;
  
  lVar3 = 0;
  if (0 < param_2) {
    do {
      iVar1 = *(int *)(param_3 + 4 + lVar3 * 4);
      iVar2 = *(int *)(param_4 + 4 + lVar3 * 4);
      if ((((iVar1 == iVar2) || (iVar1 < 1)) || (param_1 < iVar1)) ||
         ((iVar2 < 1 || (param_1 < iVar2)))) {
        nrerror_vsdlss(0x1d,"interface.c",0x263,iVar1,iVar2,param_1);
      }
      iVar1 = (int)lVar3;
      lVar3 = lVar3 + 1;
    } while (iVar1 + 2 <= param_2);
  }
  return;
}


