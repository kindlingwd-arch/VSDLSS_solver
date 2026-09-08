/* driver: KBTaEyOnQOjFPQXSPAIPP addr=0096f1d0 size=68 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void KBTaEyOnQOjFPQXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  if (DAT_00e3a0e8 < 0) {
    uVar1 = DBQkBJYIIL_ZIL_HqFWBXSPAIPP();
    FKFQayPFkyIIL_dOBBXSPAIPP(uVar1);
  }
  _DAT_00e5a520 = _DAT_00e5a520 + 1;
  if (DAT_00e5a538 == (undefined8 *)0x0) {
    DAT_00e5a538 = (undefined8 *)KBTaEyOnQOaERKHXSPAIPP();
  }
  uVar1 = DAT_00e5a538[1];
  *DAT_00e5a538 = param_1;
  DAT_00e5a538[1] = param_2;
  DAT_00e5a538 = (undefined8 *)uVar1;
  return;
}


