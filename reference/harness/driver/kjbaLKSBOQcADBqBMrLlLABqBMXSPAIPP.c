/* driver: kjbaLKSBOQcADBqBMrLlLABqBMXSPAIPP addr=0099cde0 size=e2 */
#include "vsdlss_ref.h"


undefined8 kjbaLKSBOQcADBqBMrLlLABqBMXSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  long *plVar7;
  undefined1 auStack_48 [40];
  
  lVar4 = _LJMRQBnF_HjFDEQBOqFABd(param_2,param_3);
  if (lVar4 == 0) {
    lVar4 = *(long *)(param_3 + 0x88);
    *(undefined8 *)(param_3 + 0x88) = 0;
    uVar5 = kjbeBQZFnyOQFQBeOyMEXSPAIPP(param_1,param_2,param_3,auStack_48,lVar4);
    lVar2 = *(long *)(param_3 + 0x80);
    *(undefined8 *)(param_3 + 0x80) = 0;
    lVar6 = kjbkFKaLSBOXSPAIPP(auStack_48,uVar5,param_3,lVar2);
    plVar7 = (long *)(lVar2 + 8);
    lVar3 = *(long *)(param_3 + 0xa0);
    plVar1 = plVar7 + lVar6;
    if (0 < lVar6) {
      do {
        lVar6 = *plVar7;
        plVar7 = plVar7 + 1;
        *(undefined8 *)(lVar3 + *(long *)(lVar4 + lVar6 * 8) * 8) = 3;
      } while (plVar7 != plVar1);
    }
    *(long *)(param_3 + 0x80) = lVar2;
    *(long *)(param_3 + 0x88) = lVar4;
  }
  else {
    MF_HjFDEQBOqFABYPqBM(param_2,param_3);
  }
  kjbaLJMRQBeOyMElLABnyOQFQFLKbBQyFIPX1(param_2,param_3);
  kjbdklLABpBCFKBXE(param_2,param_3);
  return *(undefined8 *)(param_2 + 0x20);
}


