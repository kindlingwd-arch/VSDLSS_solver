/* driver: EyKAIBgPLIyQBAtyOPgKbFyDtB_XSPAIPP addr=0096eb60 size=3d */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void EyKAIBgPLIyQBAtyOPgKbFyDtB_XSPAIPP(long param_1,long param_2,long param_3)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  
  if (0 < param_1 - param_3) {
    pdVar1 = (double *)(param_2 + 8) + (param_1 - param_3);
    pdVar2 = (double *)(param_2 + 8);
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


