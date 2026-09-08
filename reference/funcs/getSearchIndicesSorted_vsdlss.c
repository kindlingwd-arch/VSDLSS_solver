/* VSDLSS function (decompiled by Ghidra) */
/* name: getSearchIndicesSorted_vsdlss  addr: 009e88a0  size: cb */
#include "vsdlss_ref.h"

/* signature: void getSearchIndicesSorted_vsdlss(long param_1,int param_2,int *param_3,int param_4,long param_5); */

void getSearchIndicesSorted_vsdlss(long param_1,int param_2,int *param_3,int param_4,long param_5)

{
  int iVar1;
  int iVar2;
  long lVar3;
  
  if (param_4 < param_2) {
    nrerror_vsdlss(0x25,"factora.c",0xae);
  }
  if ((param_2 < 1) || (param_4 < 1)) {
    iVar2 = 1;
  }
  else {
    iVar1 = 1;
    iVar2 = 1;
    do {
      param_3 = param_3 + 1;
      lVar3 = (long)iVar2;
      if (*(int *)(param_1 + lVar3 * 4) == *param_3) {
        iVar2 = iVar2 + 1;
        *(int *)(param_5 + lVar3 * 4) = iVar1;
      }
    } while ((iVar2 <= param_2) && (iVar1 = iVar1 + 1, iVar1 <= param_4));
  }
  if (param_2 < iVar2) {
    return;
  }
  nrerror_vsdlss(0x25,"factora.c",0xbc);
  return;
}


