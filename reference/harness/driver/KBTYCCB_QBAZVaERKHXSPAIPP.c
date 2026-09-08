/* driver: KBTYCCB_QBAZVaERKHXSPAIPP addr=0096ee10 size=a2 */
#include "vsdlss_ref.h"


long * KBTYCCB_QBAZVaERKHXSPAIPP(long param_1)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  plVar2 = (long *)_yPFkyIIL_XSPAIPP(0x18);
  *plVar2 = param_1;
  lVar3 = KBTatB_QLOXSPAIPP(((param_1 + -1) * param_1) / 2,1,0);
  plVar2[1] = lVar3;
  lVar3 = KBTantB_QLOXSPAIPP(*plVar2,1,0);
  plVar2[2] = lVar3;
  if (1 < param_1) {
    lVar1 = *plVar2;
    *(long *)(lVar3 + 0x10) = plVar2[1];
    if (2 < lVar1) {
      lVar3 = 0x10;
      lVar4 = 1;
      do {
        lVar5 = lVar4 + 1;
        *(long *)(plVar2[2] + 8 + lVar3) = lVar4 + *(long *)(plVar2[2] + lVar3);
        lVar3 = lVar3 + 8;
        lVar4 = lVar5;
      } while (lVar5 != lVar1 + -1);
    }
  }
  return plVar2;
}


