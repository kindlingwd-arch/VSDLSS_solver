/* driver: KNZMNXSPAIPP addr=009b24b0 size=a1 */
#include "vsdlss_ref.h"


void KNZMNXSPAIPP(long param_1,long param_2,long *param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar3 = *param_3;
  if ((((param_1 < 1) || (lVar3 < param_1)) || (param_2 < 1)) || (lVar3 < param_2)) {
    KOBOOLOXSPAIPP(1,"bpqutils.c",0x104);
  }
  lVar4 = param_3[7];
  plVar1 = (long *)(param_3[6] + param_2 * 8);
  plVar2 = (long *)(param_3[8] + param_1 * 8);
  *plVar2 = *plVar1;
  *(long *)(lVar4 + param_1 * 8) = param_2 + lVar3;
  *plVar1 = param_1;
  lVar5 = *plVar2;
  if (param_3[4] != lVar5) {
    if ((lVar3 < lVar5) || (lVar5 < 1)) {
      KOBOOLOXSPAIPP(1,"bpqutils.c",0x111);
    }
    *(long *)(lVar4 + lVar5 * 8) = param_1;
  }
  param_3[2] = param_3[2] + 1;
  return;
}


