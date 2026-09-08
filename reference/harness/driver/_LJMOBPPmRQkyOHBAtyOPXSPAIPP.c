/* driver: _LJMOBPPmRQkyOHBAtyOPXSPAIPP addr=00961ff0 size=31 */
#include "vsdlss_ref.h"


long _LJMOBPPmRQkyOHBAtyOPXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  if (0 < param_2) {
    lVar3 = 1;
    lVar2 = 0;
    do {
      lVar1 = *(long *)(param_3 + lVar3 * 8);
      if (*(char *)(param_1 + lVar1) == '\0') {
        lVar2 = lVar2 + 1;
        *(long *)(param_3 + lVar2 * 8) = lVar1;
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 != param_2 + 1);
    return lVar2;
  }
  return 0;
}


