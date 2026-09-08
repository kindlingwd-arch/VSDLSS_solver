/* VSDLSS function (decompiled by Ghidra) */
/* name: printTimeDeciSeconds_vsdlss  addr: 009d8dd0  size: 2d */
#include "vsdlss_ref.h"

/* signature: void printTimeDeciSeconds_vsdlss(undefined8 *param_1,undefined8 param_2); */

void printTimeDeciSeconds_vsdlss(undefined8 *param_1,undefined8 param_2)

{
  convertToDeciSeconds_vsdlss();
  printf("%s:uTime:%ld:sTime:%ld\n",param_2,*param_1,param_1[1]);
  return;
}


