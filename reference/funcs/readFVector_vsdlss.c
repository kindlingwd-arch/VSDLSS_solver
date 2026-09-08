/* VSDLSS function (decompiled by Ghidra) */
/* name: readFVector_vsdlss  addr: 00a0dd60  size: ba */
#include "vsdlss_ref.h"

/* signature: void * readFVector_vsdlss(FILE *param_1,int *param_2); */

void * readFVector_vsdlss(FILE *param_1,int *param_2)

{
  size_t sVar1;
  void *__ptr;
  int local_1c;
  
  sVar1 = fread(&local_1c,4,1,param_1);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xbc,"readFVector:n",1,sVar1 & 0xffffffff);
  }
  __ptr = (void *)newFVector_vsdlss(0,(long)local_1c,0);
  sVar1 = fread(__ptr,8,(long)(local_1c + 1),param_1);
  if (local_1c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xc0,"readFVector:v",local_1c + 1,sVar1 & 0xffffffff);
  }
  *param_2 = local_1c;
  return __ptr;
}


