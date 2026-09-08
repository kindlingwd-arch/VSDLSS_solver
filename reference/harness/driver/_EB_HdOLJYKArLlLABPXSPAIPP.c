/* driver: _EB_HdOLJYKArLlLABPXSPAIPP addr=0098ab60 size=73 */
#include "vsdlss_ref.h"


void _EB_HdOLJYKArLlLABPXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = 1;
  if (0 < param_2) {
    do {
      lVar1 = *(long *)(param_3 + lVar3 * 8);
      lVar2 = *(long *)(param_4 + lVar3 * 8);
      if ((((lVar1 == lVar2) || (lVar1 < 1)) || (param_1 < lVar1)) ||
         ((lVar2 < 1 || (param_1 < lVar2)))) {
        KOBOOLOXSPAIPP(0x1d,"interface.c",0x263,lVar1,lVar2,param_1);
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 <= param_2);
  }
  return;
}


