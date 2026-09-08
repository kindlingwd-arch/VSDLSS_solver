/* driver: CIRPEnNXSPAIPP addr=009a6e60 size=2a */
#include "vsdlss_ref.h"


void CIRPEnNXSPAIPP(long param_1)

{
  long lVar1;
  undefined1 local_10 [8];
  
  lVar1 = *(long *)(param_1 + 0x10);
  while (lVar1 != 0) {
    ANnNXSPAIPP(param_1,local_10);
    lVar1 = *(long *)(param_1 + 0x10);
  }
  return;
}


