/* VSDLSS function (decompiled by Ghidra) */
/* name: getFactorChunkFullFn_vsdlss  addr: 009e2630  size: 8f */
#include "vsdlss_ref.h"

/* signature: void getFactorChunkFullFn_vsdlss(long param_1,int param_2,char *param_3); */

void getFactorChunkFullFn_vsdlss(long param_1,int param_2,char *param_3)

{
  char *__s;
  size_t sVar1;
  int iVar2;
  undefined1 auStack_b8 [152];
  
  sVar1 = strlen(baseJobName_vsdlss);
  iVar2 = 0x3ee - (int)sVar1;
  __s = *(char **)(*(long *)(param_1 + 0x18) +
                  (long)*(int *)(*(long *)(param_1 + 0x20) + (long)param_2 * 4) * 8);
  sVar1 = strlen(__s);
  if (iVar2 < (int)sVar1) {
    nrerror_vsdlss(0x12,"chunkinfoutils.c",0x99,sVar1 & 0xffffffff,iVar2);
  }
  getFactorChunkFn_vsdlss(param_2,auStack_b8);
  sprintf(param_3,"%s%s",__s,auStack_b8);
  return;
}


