/* VSDLSS function (decompiled by Ghidra) */
/* name: matMatMult_casi_vsdlss  addr: 009ebc20  size: 8d */
#include "vsdlss_ref.h"

/* signature: void matMatMult_casi_vsdlss(undefined8 param_1,int param_2,undefined4 param_3,undefined8 param_4,undefined4 param_5,undefined8 param_6,int param_7); */

void matMatMult_casi_vsdlss
               (undefined8 param_1,int param_2,undefined4 param_3,undefined8 param_4,
               undefined4 param_5,undefined8 param_6,int param_7)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_7 == 0) {
    matMatMult_casi_destRange(param_1);
    return;
  }
  iVar2 = 1;
  if (0 < param_2) {
    do {
      iVar1 = iVar2 + 0xb;
      if (param_2 <= iVar2 + 0xb) {
        iVar1 = param_2;
      }
      iVar3 = iVar2 + 0xc;
      matMatMult_casi_srcRange_1(param_1,param_2,param_3,param_4,param_5,param_6,iVar2,iVar1);
      iVar2 = iVar3;
    } while (iVar3 <= param_2);
  }
  return;
}


