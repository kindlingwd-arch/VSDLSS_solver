/* driver: OByAgtB_QLOXSPAIPP addr=009a8570 size=ba */
#include "vsdlss_ref.h"


void * OByAgtB_QLOXSPAIPP(FILE *param_1,long *param_2)

{
  size_t sVar1;
  void *__ptr;
  long local_20;
  
  sVar1 = fread(&local_20,8,1,param_1);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0xef,"OByAgtB_QLO:n",1,sVar1);
  }
  __ptr = (void *)KBTgtB_QLOXSPAIPP(local_20,0,0);
  sVar1 = fread(__ptr,8,local_20 + 1,param_1);
  if (local_20 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0xf4,"OByAgtB_QLO:v",local_20 + 1U,sVar1);
  }
  *param_2 = local_20;
  return __ptr;
}


