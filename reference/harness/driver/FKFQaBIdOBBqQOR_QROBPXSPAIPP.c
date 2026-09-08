/* driver: FKFQaBIdOBBqQOR_QROBPXSPAIPP addr=00974010 size=38 */
#include "vsdlss_ref.h"


void FKFQaBIdOBBqQOR_QROBPXSPAIPP(ulong param_1)

{
  DAT_00e3a0f8 = _OByQBryDXSPAIPP();
  DAT_00e74ed8 = 0;
  DAT_00e74ed0 = 1;
  if (0x1f < param_1) {
    DAT_00e74ed0 = param_1 >> 4;
  }
  return;
}


