/* driver: FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP addr=00961a70 size=c2 */
#include "vsdlss_ref.h"


void FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP(long param_1,ulong param_2,long param_3)

{
  long lVar1;
  ulong uVar2;
  long lVar3;
  
  lVar3 = (param_2 + 1) * 8;
  lVar1 = _yPFkyIIL_XSPAIPP(lVar3);
  if (lVar1 == 0) {
    KOBOOLOXSPAIPP(0xd,"utils.c",0x63a,lVar3,"polyNewVectorNoInit()");
  }
  uVar2 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(lVar1 + uVar2 * 8) =
           *(undefined8 *)(param_3 + *(long *)(param_1 + uVar2 * 8) * 8);
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_2);
    lVar3 = 1;
    do {
      *(undefined8 *)(param_3 + lVar3 * 8) = *(undefined8 *)(lVar1 + lVar3 * 8);
      lVar3 = lVar3 + 1;
    } while (param_2 + 1 != lVar3);
  }
  if (lVar1 != 0) {
    _yPFdOBBXSPAIPP(lVar1);
    return;
  }
  return;
}


