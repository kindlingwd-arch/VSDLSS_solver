/* driver: ABIBQBcKQFOBacADBIFPQXSPAIPP addr=009741c0 size=2b */
#include "vsdlss_ref.h"


undefined8 ABIBQBcKQFOBacADBIFPQXSPAIPP(long param_1)

{
  long lVar1;
  
  while (lVar1 = param_1, lVar1 != 0) {
    param_1 = *(long *)(lVar1 + 8);
    *(long *)(lVar1 + 8) = DAT_00e74ed8;
    DAT_00e74ed8 = lVar1;
  }
  return 0;
}


