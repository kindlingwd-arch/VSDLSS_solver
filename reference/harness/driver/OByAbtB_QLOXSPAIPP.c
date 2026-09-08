/* driver: OByAbtB_QLOXSPAIPP addr=009a8290 size=bc */
#include "vsdlss_ref.h"


void * OByAbtB_QLOXSPAIPP(FILE *param_1,long *param_2)

{
  size_t sVar1;
  void *__ptr;
  long local_20;
  
  sVar1 = fread(&local_20,8,1,param_1);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x89,"readDVector:n",1,sVar1);
  }
  __ptr = (void *)KBTtB_QLOXSPAIPP(0,local_20,0);
  sVar1 = fread(__ptr,8,local_20 + 1,param_1);
  if (local_20 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x8d,"readDVector:v",local_20 + 1U,sVar1);
  }
  *param_2 = local_20;
  return __ptr;
}


