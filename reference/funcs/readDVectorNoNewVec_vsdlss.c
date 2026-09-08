/* VSDLSS function (decompiled by Ghidra) */
/* name: readDVectorNoNewVec_vsdlss  addr: 00a0dcc0  size: 9e */
#include "vsdlss_ref.h"

/* signature: int readDVectorNoNewVec_vsdlss(FILE *param_1,void *param_2); */

int readDVectorNoNewVec_vsdlss(FILE *param_1,void *param_2)

{
  size_t sVar1;
  int local_1c [3];
  
  sVar1 = fread(local_1c,4,1,param_1);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xa3,"readDVector:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(param_2,8,(long)(local_1c[0] + 1),param_1);
  if (local_1c[0] + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0xa6,"readDVector:v",local_1c[0] + 1,sVar1 & 0xffffffff);
  }
  return local_1c[0];
}


