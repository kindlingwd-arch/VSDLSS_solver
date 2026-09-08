/* driver: yAGRPQdLOgPLIyQBAtyOPXSPAIPP addr=0096eb10 size=3f */
#include "vsdlss_ref.h"


void yAGRPQdLOgPLIyQBAtyOPXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar2 = *(long *)(param_1 + 0x48);
  lVar3 = 1;
  lVar1 = *(long *)(param_1 + 8) + 1;
  if (0 < *(long *)(param_1 + 8)) {
    do {
      while (0.0 < *(double *)(lVar2 + lVar3 * 8)) {
        lVar3 = lVar3 + 1;
        if (lVar3 == lVar1) {
          return;
        }
      }
      *(undefined8 *)(param_2 + lVar3 * 8) = 0;
      lVar3 = lVar3 + 1;
    } while (lVar3 != lVar1);
  }
  return;
}


