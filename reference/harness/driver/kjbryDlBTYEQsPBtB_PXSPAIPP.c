/* driver: kjbryDlBTYEQsPBtB_PXSPAIPP addr=009b3230 size=129 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long * kjbryDlBTYEQsPBtB_PXSPAIPP
                 (undefined8 param_1,long param_2,long param_3,long param_4,long param_5,
                 long param_6)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  double dVar6;
  long local_40;
  
  lVar5 = 2;
  if (1 < param_2) {
    lVar5 = param_2;
  }
  if (lVar5 < 0x10) {
    lVar4 = 1;
  }
  else {
    dVar6 = log((double)lVar5 * DAT_00aba108);
    dVar6 = floor(dVar6 / _DAT_00b1ddf8);
    lVar4 = (long)dVar6;
    if (lVar4 < 1) {
      lVar3 = 1;
      local_40 = 0;
      goto LAB_009b3288;
    }
  }
  lVar1 = 1;
  lVar3 = 1;
  do {
    lVar1 = lVar1 + 1;
    lVar3 = lVar3 * 2;
  } while (lVar1 <= lVar4);
  local_40 = lVar3 + -1;
LAB_009b3288:
  plVar2 = (long *)KBTZIL_HXSPAIPP(param_1,0x38);
  *plVar2 = lVar5;
  plVar2[1] = lVar3;
  plVar2[3] = param_3;
  plVar2[2] = local_40;
  FKFQgtB_QLOXSPAIPP(param_3,lVar3,0);
  plVar2[4] = param_4;
  FKFQgntB_QLOXSPAIPP(param_4,lVar3,0);
  plVar2[5] = param_5;
  plVar2[6] = param_6;
  return plVar2;
}


