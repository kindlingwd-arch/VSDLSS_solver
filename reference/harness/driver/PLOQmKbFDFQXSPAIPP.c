/* driver: PLOQmKbFDFQXSPAIPP addr=00979070 size=15a */
#include "vsdlss_ref.h"


void PLOQmKbFDFQXSPAIPP(long param_1,long param_2,ulong param_3,long param_4,long param_5,
                       long param_6,long param_7)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  
  FKFQgtB_QLOXSPAIPP(param_5,param_3,0);
  plVar2 = (long *)(param_1 + 8);
  plVar4 = plVar2 + param_2;
  if (0 < param_2) {
    do {
      lVar3 = *plVar2;
      plVar2 = plVar2 + 1;
      plVar1 = (long *)(param_5 + lVar3 * 8);
      *plVar1 = *plVar1 + 1;
    } while (plVar2 != plVar4);
  }
  if ((0 < (long)param_3) && (*(undefined8 *)(param_6 + 8) = 1, 1 < param_3)) {
    lVar3 = 2;
    do {
      *(long *)(param_6 + lVar3 * 8) =
           *(long *)(param_6 + -8 + lVar3 * 8) + *(long *)(param_5 + -8 + lVar3 * 8);
      lVar3 = lVar3 + 1;
    } while (lVar3 != param_3 + 1);
  }
  plVar4 = (long *)(param_4 + 8);
  plVar2 = plVar4 + param_2;
  if (0 < param_2) {
    do {
      lVar3 = *plVar4;
      plVar4 = plVar4 + 1;
      plVar1 = (long *)(param_6 + *(long *)(param_1 + lVar3 * 8) * 8);
      *(long *)(param_7 + *plVar1 * 8) = lVar3;
      *plVar1 = *plVar1 + 1;
    } while (plVar4 != plVar2);
  }
  if ((*(long *)(param_6 + 8) != *(long *)(param_5 + 8) + 1) ||
     (*(long *)(param_6 + param_3 * 8) != param_2 + 1)) {
    KOBOOLOXSPAIPP(1,"elemarrutils.c",0x110);
  }
  lVar3 = 2;
  if (1 < (long)param_3) {
    do {
      if (*(long *)(param_6 + lVar3 * 8) !=
          *(long *)(param_5 + lVar3 * 8) + *(long *)(param_6 + -8 + lVar3 * 8)) {
        KOBOOLOXSPAIPP(1,"elemarrutils.c",0x115);
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 <= (long)param_3);
  }
  _LMVgtB_QLOXSPAIPP(param_7,param_2,param_4);
  return;
}


