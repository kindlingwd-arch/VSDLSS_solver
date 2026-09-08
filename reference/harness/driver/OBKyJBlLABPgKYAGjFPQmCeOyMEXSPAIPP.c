/* driver: OBKyJBlLABPgKYAGjFPQmCeOyMEXSPAIPP addr=00989120 size=42 */
#include "vsdlss_ref.h"


void OBKyJBlLABPgKYAGjFPQmCeOyMEXSPAIPP(long *param_1,long param_2)

{
  undefined8 *puVar1;
  long *plVar2;
  undefined8 *puVar3;
  
  if (*param_1 < 1) {
    return;
  }
  puVar3 = (undefined8 *)(param_1[4] + 8);
  puVar1 = puVar3 + *param_1;
  do {
    for (plVar2 = (long *)*puVar3; plVar2 != (long *)0x0; plVar2 = (long *)plVar2[1]) {
      *plVar2 = *(long *)(param_2 + *plVar2 * 8);
    }
    puVar3 = puVar3 + 1;
  } while (puVar3 != puVar1);
  return;
}


