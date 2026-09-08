/* driver: _LKSBOQdOLJlLABrLlLKxBOLPXSPAIPP addr=00979560 size=72 */
#include "vsdlss_ref.h"


void _LKSBOQdOLJlLABrLlLKxBOLPXSPAIPP(long *param_1)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  
  lVar3 = *param_1;
  lVar4 = param_1[5];
  lVar5 = KBTgtB_QLOXSPAIPP(param_1[1],1,0);
  plVar6 = (long *)(lVar4 + 8);
  plVar1 = plVar6 + lVar3;
  if (0 < lVar3) {
    do {
      lVar3 = *plVar6;
      plVar6 = plVar6 + 1;
      plVar2 = (long *)(lVar5 + lVar3 * 8);
      *plVar2 = *plVar2 + 1;
    } while (plVar6 != plVar1);
  }
  if (param_1[5] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[5] = 0;
  param_1[6] = lVar5;
  *(undefined1 *)(param_1 + 4) = 1;
  return;
}


