/* driver: KBTYEQXSPAIPP addr=009af560 size=ed */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long * KBTYEQXSPAIPP(long param_1)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  double dVar6;
  
  lVar3 = 2;
  if (1 < param_1) {
    lVar3 = param_1;
  }
  if (lVar3 < 0x10) {
    lVar4 = 1;
  }
  else {
    dVar6 = log((double)lVar3 * DAT_00aba108);
    dVar6 = floor(dVar6 / _DAT_00b1ddf8);
    lVar4 = (long)dVar6;
    if (lVar4 < 1) {
      lVar5 = 1;
      lVar4 = 0;
      goto LAB_009af5a3;
    }
  }
  lVar1 = 1;
  lVar5 = 1;
  do {
    lVar1 = lVar1 + 1;
    lVar5 = lVar5 * 2;
  } while (lVar1 <= lVar4);
  lVar4 = lVar5 + -1;
LAB_009af5a3:
  plVar2 = (long *)_yPFkyIIL_XSPAIPP(0x38);
  *plVar2 = lVar3;
  plVar2[1] = lVar5;
  plVar2[2] = lVar4;
  lVar4 = KBTgtB_QLOXSPAIPP(lVar5,1,0);
  plVar2[3] = lVar4;
  lVar4 = KBTgntB_QLOXSPAIPP(lVar5,1,0);
  plVar2[4] = lVar4;
  lVar4 = KBTgtB_QLOXSPAIPP(lVar3,0,0);
  plVar2[5] = lVar4;
  lVar3 = KBTgtB_QLOXSPAIPP(lVar3,0,0);
  plVar2[6] = lVar3;
  return plVar2;
}


