/* driver: TOFQBgKFQpBLOABOtB_XSPAIPP addr=009a8c70 size=b9 */
#include "vsdlss_ref.h"


void TOFQBgKFQpBLOABOtB_XSPAIPP(void *param_1,long param_2)

{
  FILE *__s;
  size_t sVar1;
  long local_20;
  
  __s = (FILE *)LMBKdFIBuXSPAIPP(7);
  local_20 = param_2;
  sVar1 = fwrite(&local_20,8,1,__s);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x217,"writeIVector:n",1,sVar1);
  }
  sVar1 = fwrite(param_1,8,local_20 + 1,__s);
  if (local_20 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x217,"writeIVector:v",local_20 + 1U,sVar1);
  }
  _yPFdaILPBXSPAIPP(7);
  return;
}


