/* driver: kjbdFKFPEpBLOABOFKDXSPAIPP addr=009b5fe0 size=d0 */
#include "vsdlss_ref.h"


void kjbdFKFPEpBLOABOFKDXSPAIPP(long param_1,long param_2,long param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  long lVar4;
  long lVar5;
  
  lVar1 = param_4[2];
  lVar2 = *param_4;
  if (param_1 != param_2) {
    KOBOOLOXSPAIPP(1,"mldmmd.c",0x719);
  }
  lVar5 = 1;
  if (0 < param_1) {
    do {
      while( true ) {
        lVar4 = *(long *)(param_3 + lVar5 * 8);
        if (lVar4 < 0) {
          lVar4 = -lVar4;
          *(long *)(param_3 + lVar5 * 8) = lVar4;
        }
        if ((lVar4 <= param_1) && (0 < lVar4)) break;
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x724);
        plVar3 = (long *)(lVar1 + lVar4 * 8);
        if (*plVar3 < lVar2) goto LAB_009b603f;
LAB_009b6079:
        lVar5 = lVar5 + 1;
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x72d);
        if (param_1 < lVar5) goto LAB_009b6098;
      }
      plVar3 = (long *)(lVar1 + lVar4 * 8);
      if (lVar2 <= *plVar3) goto LAB_009b6079;
LAB_009b603f:
      lVar5 = lVar5 + 1;
      *plVar3 = lVar2;
    } while (lVar5 <= param_1);
  }
LAB_009b6098:
  RKJyOHaLRKQFKDkyOHXSPAIPP(param_4);
  return;
}


