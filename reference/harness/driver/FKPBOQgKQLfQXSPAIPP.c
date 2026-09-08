/* driver: FKPBOQgKQLfQXSPAIPP addr=009b29a0 size=29 */
#include "vsdlss_ref.h"


void FKPBOQgKQLfQXSPAIPP(long param_1,undefined8 param_2,long *param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)(param_3[1] + 8 + ((param_1 + -1) % *param_3) * 8);
  uVar2 = KBTacADBIFPQXSPAIPP(param_2,*puVar1);
  *puVar1 = uVar2;
  return;
}


