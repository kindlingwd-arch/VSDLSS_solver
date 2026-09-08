/* driver: ABIBQBZR_HBQdOLJfQXSPAIPP addr=009b2920 size=20 */
#include "vsdlss_ref.h"


void ABIBQBZR_HBQdOLJfQXSPAIPP(long param_1,long *param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)(param_2[1] + 8 + ((param_1 + -1) % *param_2) * 8);
  uVar2 = ABIBQBcKQFOBacADBIFPQXSPAIPP(*puVar1);
  *puVar1 = uVar2;
  return;
}


