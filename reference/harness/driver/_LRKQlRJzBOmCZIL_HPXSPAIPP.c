/* driver: _LRKQlRJzBOmCZIL_HPXSPAIPP addr=009898c0 size=36 */
#include "vsdlss_ref.h"


long _LRKQlRJzBOmCZIL_HPXSPAIPP(long *param_1)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  
  if (0 < *param_1) {
    plVar3 = (long *)(param_1[1] + 8);
    plVar1 = plVar3 + *param_1;
    lVar2 = 0;
    do {
      lVar2 = (lVar2 + 1) - (ulong)(*plVar3 == 0);
      plVar3 = plVar3 + 1;
    } while (plVar3 != plVar1);
    return lVar2;
  }
  return 0;
}


