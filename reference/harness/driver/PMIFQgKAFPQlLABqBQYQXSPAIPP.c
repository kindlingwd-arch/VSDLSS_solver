/* driver: PMIFQgKAFPQlLABqBQYQXSPAIPP addr=009894a0 size=f1 */
#include "vsdlss_ref.h"


void PMIFQgKAFPQlLABqBQYQXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  
  if (param_2 == param_3) {
    KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0x4b);
  }
  plVar1 = *(long **)(*(long *)(param_1 + 8) + param_2 * 8);
  if ((plVar1 == (long *)0x0) || (*(long *)(*(long *)(param_1 + 8) + param_3 * 8) != 0)) {
    KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0x55);
  }
  else if (*plVar1 != param_2) {
    KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0x5a);
  }
  plVar2 = (long *)plVar1[1];
  if (plVar2 != (long *)0x0) {
    lVar3 = *plVar2;
    while( true ) {
      if (lVar3 == param_3) {
        lVar3 = *(long *)(param_1 + 8);
        plVar1[1] = 0;
        *(long **)(lVar3 + param_3 * 8) = plVar2;
        return;
      }
      plVar4 = (long *)plVar2[1];
      if (plVar4 == (long *)0x0) break;
      lVar3 = *plVar4;
      plVar1 = plVar2;
      plVar2 = plVar4;
    }
  }
  KOBOOLOXSPAIPP(1,"indistnodesetsutils.c",0x6b);
  return;
}


