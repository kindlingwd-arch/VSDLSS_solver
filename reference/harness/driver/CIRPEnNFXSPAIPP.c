/* driver: CIRPEnNFXSPAIPP addr=009a7920 size=37 */
#include "vsdlss_ref.h"


void CIRPEnNFXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if (0 < *(long *)(param_1 + 0x10)) {
    lVar2 = *(long *)(param_1 + 0x20);
    lVar3 = *(long *)(param_1 + 0x18);
    lVar4 = 1;
    do {
      lVar1 = lVar4 * 8;
      lVar4 = lVar4 + 1;
      *(undefined8 *)(lVar3 + *(long *)(lVar2 + lVar1) * 8) = 0xffffffffffffffff;
    } while (lVar4 <= *(long *)(param_1 + 0x10));
  }
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}


