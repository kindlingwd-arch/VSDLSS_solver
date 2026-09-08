/* driver: OByAbtB_QLOlLlBTtB_XSPAIPP addr=009a8350 size=a1 */
#include "vsdlss_ref.h"


long OByAbtB_QLOlLlBTtB_XSPAIPP(FILE *param_1,void *param_2)

{
  size_t sVar1;
  long local_20 [2];
  
  sVar1 = fread(local_20,8,1,param_1);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0xa3,"readDVector:n",1,sVar1);
  }
  sVar1 = fread(param_2,8,local_20[0] + 1,param_1);
  if (local_20[0] + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0xa6,"readDVector:v",local_20[0] + 1U,sVar1);
  }
  return local_20[0];
}


