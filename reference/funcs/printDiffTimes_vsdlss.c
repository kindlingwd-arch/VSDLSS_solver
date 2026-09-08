/* VSDLSS function (decompiled by Ghidra) */
/* name: printDiffTimes_vsdlss  addr: 009d8d70  size: 5d */
#include "vsdlss_ref.h"

/* signature: void printDiffTimes_vsdlss(ulong *param_1,long *param_2,undefined8 param_3); */

void printDiffTimes_vsdlss(ulong *param_1,long *param_2,undefined8 param_3)

{
  long lVar1;
  ulong uVar2;
  
  uVar2 = (long)(*param_2 - *param_1) >> 0x3f;
  *param_1 = *param_2 - *param_1 ^ uVar2;
  lVar1 = param_2[1];
  *param_1 = *param_1 - uVar2;
  uVar2 = (long)(lVar1 - param_1[1]) >> 0x3f;
  param_1[1] = lVar1 - param_1[1] ^ uVar2;
  param_1[1] = param_1[1] - uVar2;
  convertToDeciSeconds_vsdlss();
  printf("%s:uTime:%ld:sTime:%ld\n",param_3,*param_1,param_1[1]);
  return;
}


