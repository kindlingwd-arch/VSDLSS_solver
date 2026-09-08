/* driver: KBTaEyOnQOaERKHXSPAIPP addr=0096f090 size=64 */
#include "vsdlss_ref.h"


void KBTaEyOnQOaERKHXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar3 = KBTZIL_HXSPAIPP(DAT_00e3a0e8,DAT_00e5a530 << 4);
  lVar2 = DAT_00e5a530;
  if (DAT_00e5a530 != 1) {
    lVar1 = DAT_00e5a530 + -1;
    lVar4 = 0;
    lVar5 = lVar3;
    do {
      lVar4 = lVar4 + 1;
      *(long *)(lVar5 + 8) = lVar5 + 0x10;
      lVar5 = lVar5 + 0x10;
    } while (lVar4 != lVar1);
    lVar3 = lVar3 + -0x10 + lVar2 * 0x10;
  }
  *(undefined8 *)(lVar3 + 8) = 0;
  return;
}


