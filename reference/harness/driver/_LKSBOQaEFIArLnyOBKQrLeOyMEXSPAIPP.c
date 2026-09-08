/* driver: _LKSBOQaEFIArLnyOBKQrLeOyMEXSPAIPP addr=00989280 size=89 */
#include "vsdlss_ref.h"


long _LKSBOQaEFIArLnyOBKQrLeOyMEXSPAIPP(long param_1,long param_2)

{
  undefined8 *puVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  
  lVar3 = KBTeOyMEXSPAIPP(param_2);
  uVar4 = KBTacADBtB_QLOXSPAIPP(param_2,1,0);
  *(undefined8 *)(lVar3 + 0x20) = uVar4;
  if (param_2 < 1) {
    lVar6 = 0;
  }
  else {
    lVar5 = 1;
    lVar6 = 0;
    do {
      lVar2 = *(long *)(param_1 + lVar5 * 8);
      if (0 < lVar2) {
        lVar6 = lVar6 + 1;
        puVar1 = (undefined8 *)(*(long *)(lVar3 + 0x20) + lVar2 * 8);
        uVar4 = KBTacADBIFPQXSPAIPP(lVar5,*puVar1);
        *puVar1 = uVar4;
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 <= param_2);
  }
  *(long *)(lVar3 + 8) = lVar6;
  return lVar3;
}


