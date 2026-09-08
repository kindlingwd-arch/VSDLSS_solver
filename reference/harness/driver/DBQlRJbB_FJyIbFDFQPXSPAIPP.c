/* driver: DBQlRJbB_FJyIbFDFQPXSPAIPP addr=00962030 size=40 */
#include "vsdlss_ref.h"


long DBQlRJbB_FJyIbFDFQPXSPAIPP(ulong param_1)

{
  long lVar1;
  long lVar2;
  
  lVar1 = 1;
  lVar2 = (param_1 ^ (long)param_1 >> 0x3f) - ((long)param_1 >> 0x3f);
  if (lVar2 != 0) {
    while (lVar2 = lVar2 / 10, lVar2 != 0) {
      lVar1 = lVar1 + 1;
    }
  }
  return lVar1;
}


