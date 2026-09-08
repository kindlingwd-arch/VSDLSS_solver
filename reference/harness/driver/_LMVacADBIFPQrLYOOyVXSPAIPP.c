/* driver: _LMVacADBIFPQrLYOOyVXSPAIPP addr=009741f0 size=28 */
#include "vsdlss_ref.h"


void _LMVacADBIFPQrLYOOyVXSPAIPP(undefined8 *param_1,long param_2)

{
  undefined8 uVar1;
  long lVar2;
  
  lVar2 = 0;
  if (param_1 == (undefined8 *)0x0) {
    return;
  }
  do {
    uVar1 = *param_1;
    param_1 = (undefined8 *)param_1[1];
    lVar2 = lVar2 + 1;
    *(undefined8 *)(param_2 + lVar2 * 8) = uVar1;
  } while (param_1 != (undefined8 *)0x0);
  return;
}


