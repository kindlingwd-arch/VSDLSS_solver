/* driver: KBTbntB_QLOXSPAIPP addr=00960d70 size=7a */
#include "vsdlss_ref.h"


long KBTbntB_QLOXSPAIPP(ulong param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  lVar1 = param_1 * 8 + 8;
  lVar2 = _yPFkyIIL_XSPAIPP(lVar1);
  if (lVar2 == 0) {
    KOBOOLOXSPAIPP(0xd,"utils.c",0x34a,lVar1,"polyNewVector()");
  }
  if ((param_2 != 0) && (param_1 != 0)) {
    uVar3 = 1;
    do {
      *(undefined8 *)(lVar2 + uVar3 * 8) = param_3;
      uVar3 = uVar3 + 1;
    } while (uVar3 <= param_1);
  }
  return lVar2;
}


