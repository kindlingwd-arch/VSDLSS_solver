/* driver: kjbaLKSBOQaLSBOrLqBMyOyQLOXSPAIPP addr=0099bec0 size=2a */
#include "vsdlss_ref.h"


void kjbaLKSBOQaLSBOrLqBMyOyQLOXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long *plVar1;
  long lVar2;
  long *plVar3;
  
  plVar3 = (long *)(param_4 + 8);
  plVar1 = plVar3 + param_3;
  if (0 < param_3) {
    do {
      lVar2 = *plVar3;
      plVar3 = plVar3 + 1;
      *(undefined8 *)(param_1 + *(long *)(param_2 + lVar2 * 8) * 8) = 3;
    } while (plVar3 != plVar1);
  }
  return;
}


