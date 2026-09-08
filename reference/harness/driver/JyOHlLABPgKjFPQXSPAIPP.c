/* driver: JyOHlLABPgKjFPQXSPAIPP addr=009aa640 size=22 */
#include "vsdlss_ref.h"


void JyOHlLABPgKjFPQXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  
  plVar3 = (long *)(param_2 + 8);
  plVar1 = plVar3 + param_1;
  if (0 < param_1) {
    do {
      lVar2 = *plVar3;
      plVar3 = plVar3 + 1;
      *(undefined1 *)(param_3 + lVar2) = 1;
    } while (plVar3 != plVar1);
  }
  return;
}


