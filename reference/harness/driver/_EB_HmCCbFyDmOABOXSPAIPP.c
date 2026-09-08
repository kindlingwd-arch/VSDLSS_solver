/* driver: _EB_HmCCbFyDmOABOXSPAIPP addr=00978fc0 size=74 */
#include "vsdlss_ref.h"


void _EB_HmCCbFyDmOABOXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  
  lVar1 = *param_1;
  lVar2 = param_1[5];
  lVar3 = param_1[7];
  lVar5 = 2;
  if (1 < lVar1) {
    do {
      while ((lVar6 = *(long *)(lVar2 + -8 + lVar5 * 8), lVar4 = *(long *)(lVar2 + lVar5 * 8),
             lVar6 <= lVar4 &&
             ((lVar4 != lVar6 || (*(long *)(lVar3 + -8 + lVar5 * 8) <= *(long *)(lVar3 + lVar5 * 8))
              )))) {
        lVar5 = lVar5 + 1;
        if (lVar1 < lVar5) {
          return;
        }
      }
      lVar6 = lVar5 + 1;
      KOBOOLOXSPAIPP(0x1e,"elemarrutils.c",0xd0,lVar5);
      lVar5 = lVar6;
    } while (lVar6 <= lVar1);
  }
  return;
}


