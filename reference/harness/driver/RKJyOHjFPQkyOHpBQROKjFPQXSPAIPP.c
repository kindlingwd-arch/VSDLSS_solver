/* driver: RKJyOHjFPQkyOHpBQROKjFPQXSPAIPP addr=0098b5b0 size=31 */
#include "vsdlss_ref.h"


void RKJyOHjFPQkyOHpBQROKjFPQXSPAIPP(long param_1)

{
  long *plVar1;
  long lVar2;
  
  lVar2 = *(long *)(param_1 + 8);
  for (plVar1 = *(long **)(param_1 + 0x10); plVar1 != (long *)0x0; plVar1 = (long *)plVar1[1]) {
    *(undefined1 *)(lVar2 + *plVar1) = 0;
  }
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  return;
}


