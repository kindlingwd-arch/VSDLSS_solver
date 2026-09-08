/* driver: _LKSBOQesr2ejrXSPAIPP addr=00988f90 size=83 */
#include "vsdlss_ref.h"


void _LKSBOQesr2ejrXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  long *plVar4;
  long lVar5;
  undefined8 *puVar6;
  long *plVar7;
  long *local_30;
  
  lVar5 = *param_1;
  lVar1 = param_1[4];
  puVar6 = (undefined8 *)(lVar1 + lVar5 * 8);
  if (0 < lVar5) {
    do {
      plVar4 = (long *)*puVar6;
      plVar7 = (long *)0x0;
      while (plVar4 != (long *)0x0) {
        while (plVar3 = plVar4, lVar2 = *plVar3, lVar2 <= lVar5) {
          plVar4 = (long *)plVar3[1];
          plVar7 = plVar3;
          if ((long *)plVar3[1] == (long *)0x0) goto LAB_00988fd0;
        }
        plVar4 = (long *)PKFMYKApBQROKacADBIFPQcIBJXSPAIPP(puVar6,plVar3,plVar7,&local_30);
        plVar3 = (long *)(lVar1 + lVar2 * 8);
        lVar2 = *plVar3;
        *local_30 = lVar5;
        local_30[1] = lVar2;
        *plVar3 = (long)local_30;
      }
LAB_00988fd0:
      puVar6 = puVar6 + -1;
      lVar5 = lVar5 + -1;
    } while (lVar5 != 0);
  }
  return;
}


