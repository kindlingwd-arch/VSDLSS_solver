/* driver: OByAYKAcUQBKAbFyDtB_XSPAIPP addr=009ac670 size=60 */
#include "vsdlss_ref.h"


void OByAYKAcUQBKAbFyDtB_XSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = param_1 - param_2;
  lVar1 = KBTdtB_QLOXSPAIPP(0,param_1,0);
  OByAdtB_QLOdlRJXSPAIPP(0x1a,lVar3,lVar1);
  if (0 < param_2) {
    lVar2 = lVar1 + lVar3 * 8;
    do {
      *(undefined8 *)(lVar2 + 8) = 0;
      lVar2 = lVar2 + 8;
    } while (lVar2 != lVar1 + (param_2 + lVar3) * 8);
  }
  return;
}


