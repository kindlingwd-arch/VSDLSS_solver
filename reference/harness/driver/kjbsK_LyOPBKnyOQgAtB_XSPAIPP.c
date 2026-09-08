/* driver: kjbsK_LyOPBKnyOQgAtB_XSPAIPP addr=0099ae70 size=65 */
#include "vsdlss_ref.h"


void kjbsK_LyOPBKnyOQgAtB_XSPAIPP(ulong param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  ulong uVar5;
  
  lVar1 = *(long *)(param_3 + 0xa0);
  lVar2 = *(long *)(param_3 + 0x60);
  lVar3 = *(long *)(param_3 + 0xa8);
  lVar4 = 1;
  if (0 < (long)param_1) {
    do {
      *(undefined8 *)(lVar2 + lVar4 * 8) =
           *(undefined8 *)(lVar1 + *(long *)(param_2 + lVar4 * 8) * 8);
      *(undefined8 *)(param_2 + lVar4 * 8) =
           *(undefined8 *)(lVar3 + *(long *)(param_2 + lVar4 * 8) * 8);
      lVar4 = lVar4 + 1;
    } while (lVar4 != param_1 + 1);
  }
  uVar5 = 1;
  if (param_1 == 0) {
    return;
  }
  do {
    *(undefined8 *)(lVar1 + uVar5 * 8) = *(undefined8 *)(lVar2 + uVar5 * 8);
    uVar5 = uVar5 + 1;
  } while (uVar5 <= param_1);
  return;
}


