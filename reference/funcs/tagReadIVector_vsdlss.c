/* VSDLSS function (decompiled by Ghidra) */
/* name: tagReadIVector_vsdlss  addr: 00a0df80  size: bc */
#include "vsdlss_ref.h"

/* signature: void * tagReadIVector_vsdlss(undefined4 param_1,FILE *param_2,int *param_3); */

void * tagReadIVector_vsdlss(undefined4 param_1,FILE *param_2,int *param_3)

{
  size_t sVar1;
  void *__ptr;
  int local_1c;
  
  sVar1 = fread(&local_1c,4,1,param_2);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x101,"tagReadIVector:n",1,sVar1 & 0xffffffff);
  }
  __ptr = (void *)tagNewIVector_vsdlss(param_1,(long)local_1c,0,0);
  sVar1 = fread(__ptr,4,(long)(local_1c + 1),param_2);
  if (local_1c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x106,"tagReadIVector:v",local_1c + 1,sVar1 & 0xffffffff)
    ;
  }
  *param_3 = local_1c;
  return __ptr;
}


