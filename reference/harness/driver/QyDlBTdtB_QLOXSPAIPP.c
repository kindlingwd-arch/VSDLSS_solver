/* driver: QyDlBTdtB_QLOXSPAIPP addr=00960fc0 size=4d */
#include "vsdlss_ref.h"


void QyDlBTdtB_QLOXSPAIPP(undefined8 param_1,undefined8 param_2,ulong param_3,long param_4)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = KBTZIL_HXSPAIPP(param_2,param_3 * 8 + 8);
  if ((param_4 != 0) && (param_3 != 0)) {
    uVar2 = 1;
    do {
      *(undefined8 *)(lVar1 + uVar2 * 8) = param_1;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_3);
  }
  return;
}


