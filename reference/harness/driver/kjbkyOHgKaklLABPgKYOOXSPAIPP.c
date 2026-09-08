/* driver: kjbkyOHgKaklLABPgKYOOXSPAIPP addr=009b4020 size=2a */
#include "vsdlss_ref.h"


void kjbkyOHgKaklLABPgKYOOXSPAIPP(long param_1,long param_2,undefined8 *param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long *plVar5;
  
  plVar5 = (long *)(param_2 + 8);
  lVar2 = param_3[2];
  uVar3 = *param_3;
  plVar1 = plVar5 + param_1;
  if (0 < param_1) {
    do {
      lVar4 = *plVar5;
      plVar5 = plVar5 + 1;
      *(undefined8 *)(lVar2 + lVar4 * 8) = uVar3;
    } while (plVar5 != plVar1);
  }
  return;
}


