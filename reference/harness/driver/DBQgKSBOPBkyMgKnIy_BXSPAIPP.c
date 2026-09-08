/* driver: DBQgKSBOPBkyMgKnIy_BXSPAIPP addr=009613d0 size=7b */
#include "vsdlss_ref.h"


void DBQgKSBOPBkyMgKnIy_BXSPAIPP(long param_1,ulong param_2)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  lVar1 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  lVar2 = 1;
  if (0 < (long)param_2) {
    do {
      *(long *)(lVar1 + *(long *)(param_1 + lVar2 * 8) * 8) = lVar2;
      lVar2 = lVar2 + 1;
    } while (lVar2 != param_2 + 1);
  }
  uVar3 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(param_1 + uVar3 * 8) = *(undefined8 *)(lVar1 + uVar3 * 8);
      uVar3 = uVar3 + 1;
    } while (uVar3 <= param_2);
  }
  if (lVar1 == 0) {
    return;
  }
  _yPFdOBBXSPAIPP(lVar1);
  return;
}


