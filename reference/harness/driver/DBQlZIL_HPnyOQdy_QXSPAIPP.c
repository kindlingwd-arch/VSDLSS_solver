/* driver: DBQlZIL_HPnyOQdy_QXSPAIPP addr=0096e4d0 size=55 */
#include "vsdlss_ref.h"


long DBQlZIL_HPnyOQdy_QXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  bool bVar4;
  
  lVar3 = 0;
  if (0 < param_3) {
    if (param_1 < 1) {
LAB_0096e510:
      KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0xbc4);
      return lVar3;
    }
    lVar2 = *(long *)(param_2 + param_1 * 8);
    lVar3 = 1;
    lVar1 = param_3 - lVar2;
    bVar4 = param_3 == lVar2;
    while (!bVar4) {
      if ((bVar4 || SBORROW8(param_3,lVar2) != lVar1 < 0) || (param_1 = param_1 + -1, param_1 == 0))
      goto LAB_0096e510;
      lVar2 = lVar2 + *(long *)(param_2 + param_1 * 8);
      lVar3 = lVar3 + 1;
      lVar1 = param_3 - lVar2;
      bVar4 = lVar1 == 0;
    }
  }
  return lVar3;
}


