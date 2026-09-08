/* VSDLSS function (decompiled by Ghidra) */
/* name: rangeCheckIVector_vsdlss  addr: 009da380  size: 7d */
#include "vsdlss_ref.h"

/* signature: void rangeCheckIVector_vsdlss(int *param_1,int param_2,int param_3,int param_4,undefined8 param_5); */

void rangeCheckIVector_vsdlss(int *param_1,int param_2,int param_3,int param_4,undefined8 param_5)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 1;
  if (0 < param_2) {
    do {
      param_1 = param_1 + 1;
      iVar1 = *param_1;
      if ((iVar1 < param_3) || (param_4 < iVar1)) {
        nrerror_vsdlss(0xb,"utils.c",0x8af,iVar1,iVar2,param_5,param_3,param_4);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 <= param_2);
  }
  return;
}


