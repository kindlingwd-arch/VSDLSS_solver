/* driver: RKJyOHjFPQkyOHXSPAIPP addr=0098b5f0 size=43 */
#include "vsdlss_ref.h"


void RKJyOHjFPQkyOHXSPAIPP(long param_1)

{
  long lVar1;
  long *plVar2;
  undefined8 uVar3;
  
  plVar2 = *(long **)(param_1 + 0x10);
  lVar1 = *(long *)(param_1 + 8);
  if (plVar2 == (long *)0x0) {
    uVar3 = 0;
  }
  else {
    do {
      *(undefined1 *)(lVar1 + *plVar2) = 0;
      plVar2 = (long *)plVar2[1];
    } while (plVar2 != (long *)0x0);
    uVar3 = *(undefined8 *)(param_1 + 0x10);
  }
  uVar3 = ABIBQBcKQFOBacADBIFPQXSPAIPP(uVar3);
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = uVar3;
  return;
}


