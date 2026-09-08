/* driver: MBOJRQBdtB_QLOgKnIy_BXSPAIPP addr=00961840 size=c2 */
#include "vsdlss_ref.h"


void MBOJRQBdtB_QLOgKnIy_BXSPAIPP(long param_1,ulong param_2,long param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  
  lVar4 = (param_2 + 1) * 8;
  lVar2 = _yPFkyIIL_XSPAIPP(lVar4);
  if (lVar2 == 0) {
    KOBOOLOXSPAIPP(0xd,"utils.c",0x5fa,lVar4,"polyNewVectorNoInit()");
  }
  uVar3 = 1;
  if (param_2 != 0) {
    do {
      lVar4 = uVar3 * 8;
      lVar1 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      *(undefined8 *)(lVar2 + *(long *)(param_1 + lVar4) * 8) = *(undefined8 *)(param_3 + lVar1);
    } while (uVar3 <= param_2);
    lVar4 = 1;
    do {
      *(undefined8 *)(param_3 + lVar4 * 8) = *(undefined8 *)(lVar2 + lVar4 * 8);
      lVar4 = lVar4 + 1;
    } while (param_2 + 1 != lVar4);
  }
  if (lVar2 != 0) {
    _yPFdOBBXSPAIPP(lVar2);
    return;
  }
  return;
}


