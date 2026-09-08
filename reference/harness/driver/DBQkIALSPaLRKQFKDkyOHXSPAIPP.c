/* driver: DBQkIALSPaLRKQFKDkyOHXSPAIPP addr=00997100 size=1d */
#include "vsdlss_ref.h"


long DBQkIALSPaLRKQFKDkyOHXSPAIPP(undefined8 param_1,long param_2)

{
  long lVar1;
  
  lVar1 = *(long *)(param_2 + 0x40);
  *(undefined8 *)(lVar1 + 0x10) = *(undefined8 *)(param_2 + 0x98);
  FKFQaLRKQFKDkyOHXSPAIPP(param_1,lVar1);
  return lVar1;
}


