/* driver: QyDpByAgtB_QLOXSPAIPP addr=009a8630 size=c0 */
#include "vsdlss_ref.h"


void * QyDpByAgtB_QLOXSPAIPP(undefined8 param_1,FILE *param_2,long *param_3)

{
  size_t sVar1;
  void *__ptr;
  long local_20;
  
  sVar1 = fread(&local_20,8,1,param_2);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x101,"QyDpByAgtB_QLO:n",1,sVar1);
  }
  __ptr = (void *)QyDlBTgtB_QLOXSPAIPP(param_1,local_20,0,0);
  sVar1 = fread(__ptr,8,local_20 + 1,param_2);
  if (local_20 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x106,"QyDpByAgtB_QLO:v",local_20 + 1U,sVar1);
  }
  *param_3 = local_20;
  return __ptr;
}


