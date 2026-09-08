/* driver: DBQgKSBOPBkyMXSPAIPP addr=009613a0 size=23 */
#include "vsdlss_ref.h"


void DBQgKSBOPBkyMXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  
  lVar1 = 1;
  if (0 < param_2) {
    do {
      *(long *)(param_3 + *(long *)(param_1 + lVar1 * 8) * 8) = lVar1;
      lVar1 = lVar1 + 1;
    } while (lVar1 != param_2 + 1);
  }
  return;
}


