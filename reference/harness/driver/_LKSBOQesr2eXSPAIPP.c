/* driver: _LKSBOQesr2eXSPAIPP addr=00988e90 size=6c */
#include "vsdlss_ref.h"


void _LKSBOQesr2eXSPAIPP(long *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  long *plVar4;
  long lVar5;
  
  lVar5 = *param_1;
  lVar2 = param_1[4];
  if (0 < lVar5) {
    do {
      for (plVar4 = *(long **)(lVar2 + lVar5 * 8); plVar4 != (long *)0x0; plVar4 = (long *)plVar4[1]
          ) {
        while (*plVar4 <= lVar5) {
          plVar4 = (long *)plVar4[1];
          if (plVar4 == (long *)0x0) goto LAB_00988eef;
        }
        puVar1 = (undefined8 *)(lVar2 + *plVar4 * 8);
        uVar3 = KBTacADBIFPQXSPAIPP(lVar5,*puVar1);
        *puVar1 = uVar3;
      }
LAB_00988eef:
      lVar5 = lVar5 + -1;
    } while (lVar5 != 0);
  }
  return;
}


