/* driver: _LKSBOQe2esrXSPAIPP addr=009890a0 size=70 */
#include "vsdlss_ref.h"


void _LKSBOQe2esrXSPAIPP(long *param_1)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  long lVar4;
  long *plVar5;
  
  lVar1 = *param_1;
  lVar4 = 1;
  if (0 < lVar1) {
    do {
      plVar5 = (long *)0x0;
      plVar3 = *(long **)(param_1[4] + lVar4 * 8);
      while (plVar3 != (long *)0x0) {
        while (plVar2 = plVar3, lVar4 <= *plVar2) {
          plVar3 = (long *)plVar2[1];
          plVar5 = plVar2;
          if ((long *)plVar2[1] == (long *)0x0) goto LAB_009890e9;
        }
        plVar3 = (long *)PKFMbFPMacADBIFPQcIBJXSPAIPP(lVar4 * 8 + param_1[4],plVar2,plVar5);
      }
LAB_009890e9:
      lVar4 = lVar4 + 1;
    } while (lVar4 <= lVar1);
  }
  return;
}


