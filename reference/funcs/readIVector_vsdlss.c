/* VSDLSS function (decompiled by Ghidra) */
/* name: readIVector_vsdlss  addr: 00a0dec0  size: b8 */
#include "vsdlss_ref.h"

/* signature: void * readIVector_vsdlss(FILE *param_1,int *param_2); */

void * readIVector_vsdlss(FILE *param_1,int *param_2)

{
  size_t sVar1;
  void *__ptr;
  int local_1c;
  
  sVar1 = fread(&local_1c,4,1,param_1);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xef,"readIVector:n",1,sVar1 & 0xffffffff);
  }
  __ptr = (void *)newIVector_vsdlss((long)local_1c,0,0);
  sVar1 = fread(__ptr,4,(long)(local_1c + 1),param_1);
  if (local_1c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xf4,"readIVector:v",local_1c + 1,sVar1 & 0xffffffff);
  }
  *param_2 = local_1c;
  return __ptr;
}


