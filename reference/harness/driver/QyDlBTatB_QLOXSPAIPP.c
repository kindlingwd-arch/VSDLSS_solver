/* driver: QyDlBTatB_QLOXSPAIPP addr=00960e90 size=3a */
#include "vsdlss_ref.h"


void QyDlBTatB_QLOXSPAIPP(undefined8 param_1,ulong param_2,long param_3,undefined1 param_4)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = KBTZIL_HXSPAIPP(param_1,param_2 + 1);
  if ((param_3 != 0) && (param_2 != 0)) {
    uVar2 = 1;
    do {
      *(undefined1 *)(lVar1 + uVar2) = param_4;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_2);
  }
  return;
}


