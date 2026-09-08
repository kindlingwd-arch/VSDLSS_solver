/* VSDLSS function (decompiled by Ghidra) */
/* name: writeDVector_vsdlss  addr: 00a0d960  size: 9c */
#include "vsdlss_ref.h"

/* signature: void writeDVector_vsdlss(void *param_1,int param_2,FILE *param_3); */

void writeDVector_vsdlss(void *param_1,int param_2,FILE *param_3)

{
  size_t sVar1;
  int local_1c [3];
  
  local_1c[0] = param_2;
  sVar1 = fwrite(local_1c,4,1,param_3);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2c,"writeDVector:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fwrite(param_1,8,(long)(local_1c[0] + 1),param_3);
  if (local_1c[0] + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2e,"writeDVector:v",local_1c[0] + 1,sVar1 & 0xffffffff)
    ;
  }
  return;
}


