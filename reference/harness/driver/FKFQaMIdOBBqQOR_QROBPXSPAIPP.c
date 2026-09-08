/* driver: FKFQaMIdOBBqQOR_QROBPXSPAIPP addr=0096f100 size=38 */
#include "vsdlss_ref.h"


void FKFQaMIdOBBqQOR_QROBPXSPAIPP(ulong param_1)

{
  DAT_00e3a0e8 = _OByQBryDXSPAIPP();
  DAT_00e5a538 = 0;
  DAT_00e5a530 = 1;
  if (0x1f < param_1) {
    DAT_00e5a530 = param_1 >> 4;
  }
  return;
}


