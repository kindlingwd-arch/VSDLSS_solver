/* driver: EyKAIBgPLIyQBAtyOPgKcyXSPAIPP addr=0096ebb0 size=42 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void EyKAIBgPLIyQBAtyOPgKcyXSPAIPP(long param_1)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  
  if (0 < *(long *)(param_1 + 8)) {
    pdVar2 = (double *)(*(long *)(param_1 + 0x48) + 8);
    pdVar1 = pdVar2 + *(long *)(param_1 + 8);
    do {
      while (*pdVar2 <= 0.0) {
        pdVar3 = pdVar2 + 1;
        *pdVar2 = _DAT_00ab67f8;
        pdVar2 = pdVar3;
        if (pdVar3 == pdVar1) {
          return;
        }
      }
      pdVar2 = pdVar2 + 1;
    } while (pdVar2 != pdVar1);
  }
  return;
}


