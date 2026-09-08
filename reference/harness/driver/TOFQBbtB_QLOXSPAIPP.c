/* driver: TOFQBbtB_QLOXSPAIPP addr=009a7fe0 size=9e */
#include "vsdlss_ref.h"


void TOFQBbtB_QLOXSPAIPP(void *param_1,long param_2,FILE *param_3)

{
  size_t sVar1;
  long local_20 [2];
  
  local_20[0] = param_2;
  sVar1 = fwrite(local_20,8,1,param_3);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2c,"writeDVector:n",1,sVar1);
  }
  sVar1 = fwrite(param_1,8,local_20[0] + 1,param_3);
  if (local_20[0] + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2e,"writeDVector:v",local_20[0] + 1U,sVar1);
  }
  return;
}


