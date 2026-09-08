/* driver: _yPFnLMXSPAIPP addr=009b2830 size=2b */
#include "vsdlss_ref.h"


undefined8 _yPFnLMXSPAIPP(undefined8 *param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)param_1[1];
  if (puVar1 != (undefined8 *)param_1[2]) {
    uVar2 = *puVar1;
    param_1[1] = puVar1[1];
    ABIBQBacADBIFPQXSPAIPP(puVar1);
    return uVar2;
  }
  return *param_1;
}


