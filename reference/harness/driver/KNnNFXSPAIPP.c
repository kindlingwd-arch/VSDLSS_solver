/* driver: KNnNFXSPAIPP addr=009a7860 size=aa */
#include "vsdlss_ref.h"


void KNnNFXSPAIPP(long param_1,undefined8 param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  
  lVar11 = *(long *)(param_3 + 0x10);
  if (*(long *)(param_3 + 8) <= lVar11) {
    KOBOOLOXSPAIPP(6,"priorityqutils_i.c",0x231);
    lVar11 = *(long *)(param_3 + 0x10);
  }
  lVar3 = *(long *)(param_3 + 0x20);
  lVar4 = *(long *)(param_3 + 0x28);
  lVar11 = lVar11 + 1;
  lVar5 = *(long *)(param_3 + 0x18);
  *(long *)(param_3 + 0x10) = lVar11;
  *(long *)(lVar3 + lVar11 * 8) = param_1;
  *(undefined8 *)(lVar4 + param_1 * 8) = param_2;
  *(long *)(lVar5 + param_1 * 8) = lVar11;
  while( true ) {
    if (lVar11 < 2) {
      return;
    }
    plVar1 = (long *)(lVar3 + lVar11 * 8);
    lVar10 = lVar11 >> 1;
    plVar2 = (long *)(lVar3 + lVar10 * 8);
    lVar6 = *plVar1;
    lVar7 = *plVar2;
    lVar8 = *(long *)(lVar4 + lVar7 * 8);
    lVar9 = *(long *)(lVar4 + lVar6 * 8);
    if ((lVar8 <= lVar9) && (((lVar8 < lVar9 || (lVar7 < lVar6)) && (lVar10 != lVar11)))) break;
    *plVar1 = lVar7;
    *plVar2 = lVar6;
    *(long *)(lVar5 + *plVar1 * 8) = lVar11;
    *(long *)(lVar5 + *plVar2 * 8) = lVar10;
    lVar11 = lVar10;
  }
  return;
}


