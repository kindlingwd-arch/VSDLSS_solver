/* driver: KBTacADBIFPQXSPAIPP addr=00974090 size=68 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void KBTacADBIFPQXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  if (DAT_00e3a0f8 < 0) {
    uVar1 = DBQkBJYIIL_ZIL_HqFWBXSPAIPP();
    FKFQaBIdOBBqQOR_QROBPXSPAIPP(uVar1);
  }
  _DAT_00e74ec8 = _DAT_00e74ec8 + 1;
  if (DAT_00e74ed8 == (undefined8 *)0x0) {
    DAT_00e74ed8 = (undefined8 *)KBTacADBaERKHXSPAIPP();
  }
  uVar1 = DAT_00e74ed8[1];
  *DAT_00e74ed8 = param_1;
  DAT_00e74ed8[1] = param_2;
  DAT_00e74ed8 = (undefined8 *)uVar1;
  return;
}


