/* driver: ABIBQBacADBIFPQXSPAIPP addr=00974110 size=15 */
#include "vsdlss_ref.h"


undefined8 ABIBQBacADBIFPQXSPAIPP(long param_1)

{
  long lVar1;
  
  lVar1 = param_1;
  *(long *)(param_1 + 8) = DAT_00e74ed8;
  DAT_00e74ed8 = lVar1;
  return 0;
}


