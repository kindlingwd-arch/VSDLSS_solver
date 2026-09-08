/* driver: kjbYAGRPQaLJMqQyOQPXSPAIPP addr=00997880 size=26 */
#include "vsdlss_ref.h"


void kjbYAGRPQaLJMqQyOQPXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  
  if (0 < param_1) {
    plVar2 = (long *)(param_3 + 8);
    plVar1 = plVar2 + param_1;
    do {
      *plVar2 = *plVar2 + param_2 + -1;
      plVar2 = plVar2 + 1;
    } while (plVar2 != plVar1);
  }
  return;
}


