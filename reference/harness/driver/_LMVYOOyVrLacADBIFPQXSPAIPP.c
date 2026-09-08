/* driver: _LMVYOOyVrLacADBIFPQXSPAIPP addr=00974340 size=36 */
#include "vsdlss_ref.h"


undefined8 _LMVYOOyVrLacADBIFPQXSPAIPP(long param_1,long param_2)

{
  undefined8 uVar1;
  
  if (param_2 < 1) {
    uVar1 = 0;
  }
  else {
    uVar1 = 0;
    do {
      uVar1 = KBTacADBIFPQXSPAIPP(*(undefined8 *)(param_1 + param_2 * 8),uVar1);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return uVar1;
}


