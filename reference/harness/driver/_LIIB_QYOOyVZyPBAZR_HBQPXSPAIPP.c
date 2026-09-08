/* driver: _LIIB_QYOOyVZyPBAZR_HBQPXSPAIPP addr=009b1cc0 size=6a */
#include "vsdlss_ref.h"


void _LIIB_QYOOyVZyPBAZR_HBQPXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  
  lVar8 = 0;
  lVar7 = 1;
  lVar1 = *(long *)(param_1 + 8);
  lVar2 = *(long *)(param_1 + 0x18);
  lVar3 = *(long *)(param_1 + 0x10);
  if (0 < lVar1) {
    do {
      lVar4 = *(long *)(lVar2 + lVar7 * 8);
      lVar5 = *(long *)(lVar3 + lVar7 * 8);
      if (0 < lVar4) {
        lVar6 = 1;
        do {
          *(undefined8 *)(param_2 + lVar8 * 8 + lVar6 * 8) = *(undefined8 *)(lVar5 + lVar6 * 8);
          lVar6 = lVar6 + 1;
        } while (lVar6 != lVar4 + 1);
        lVar8 = lVar8 + lVar4;
      }
      lVar7 = lVar7 + 1;
    } while (lVar7 != lVar1 + 1);
  }
  return;
}


