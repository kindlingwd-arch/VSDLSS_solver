/* VSDLSS function (decompiled by Ghidra) */
/* name: readDVector_vsdlss  addr: 00a0dc00  size: ba */
#include "vsdlss_ref.h"

/* signature: void * readDVector_vsdlss(FILE *param_1,int *param_2); */

void * readDVector_vsdlss(FILE *param_1,int *param_2)

{
  size_t sVar1;
  void *__ptr;
  int local_1c;
  
  sVar1 = fread(&local_1c,4,1,param_1);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x89,"readDVector:n",1,sVar1 & 0xffffffff);
  }
  __ptr = (void *)newVector_vsdlss(0,(long)local_1c,0);
  sVar1 = fread(__ptr,8,(long)(local_1c + 1),param_1);
  if (local_1c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x8d,"readDVector:v",local_1c + 1,sVar1 & 0xffffffff);
  }
  *param_2 = local_1c;
  return __ptr;
}


