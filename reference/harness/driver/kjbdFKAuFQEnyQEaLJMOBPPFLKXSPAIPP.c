/* driver: kjbdFKAuFQEnyQEaLJMOBPPFLKXSPAIPP addr=009b33a0 size=a7 */
#include "vsdlss_ref.h"


long kjbdFKAuFQEnyQEaLJMOBPPFLKXSPAIPP(long param_1,long param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  
  plVar1 = (long *)(param_2 + param_1 * 8);
  lVar4 = *plVar1;
  lVar5 = param_1;
  plVar6 = plVar1;
  if (lVar4 != 0) {
    do {
      if (lVar5 == lVar4) {
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x5e);
      }
      lVar5 = *plVar6;
      plVar6 = (long *)(param_2 + lVar5 * 8);
      lVar4 = *plVar6;
    } while (lVar4 != 0);
    lVar3 = *plVar1;
    lVar4 = param_1;
    while (lVar2 = lVar3, lVar2 != 0) {
      *plVar1 = lVar5;
      plVar1 = (long *)(param_2 + lVar2 * 8);
      lVar4 = lVar2;
      lVar3 = *plVar1;
    }
    param_1 = lVar5;
    if (lVar5 != lVar4) {
      KOBOOLOXSPAIPP(1,"mldmmd.c",0x6e);
    }
  }
  return param_1;
}


