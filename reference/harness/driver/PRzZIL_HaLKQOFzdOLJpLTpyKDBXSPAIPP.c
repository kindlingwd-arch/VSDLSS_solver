/* driver: PRzZIL_HaLKQOFzdOLJpLTpyKDBXSPAIPP addr=00980880 size=13d */
#include "vsdlss_ref.h"


void PRzZIL_HaLKQOFzdOLJpLTpyKDBXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,undefined8 param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 in_stack_00000020;
  
  lVar1 = *(long *)(*(long *)(param_6 + 0x48) + param_1 * 8);
  lVar2 = *(long *)(*(long *)(param_6 + 0x50) + param_1 * 8);
  if (param_4 < param_3) {
    return;
  }
  lVar4 = (param_2 - param_1) + 2;
  while( true ) {
    if (lVar1 < lVar4) {
      return;
    }
    if (param_3 <= *(long *)(lVar2 + lVar4 * 8)) break;
    lVar4 = lVar4 + 1;
  }
  lVar5 = lVar4 + -1;
  if (lVar5 < lVar1) {
    lVar3 = *(long *)(lVar2 + lVar4 * 8);
    while (lVar3 <= param_4) {
      lVar5 = lVar5 + 1;
      if (lVar1 <= lVar5) goto LAB_00980934;
      lVar3 = *(long *)(lVar2 + 8 + lVar5 * 8);
    }
    if (lVar5 < lVar4) {
      return;
    }
  }
LAB_00980934:
  _LMVqRMlLABtyIPrLjLKDtB_XSPAIPP(param_1,param_2,lVar4,param_6,in_stack_00000020);
  PRzZIL_HaLKQOFzdOLJpLTpyKDBX1XSPAIPP(param_1,param_2,lVar4,lVar5,param_5,param_6);
  return;
}


