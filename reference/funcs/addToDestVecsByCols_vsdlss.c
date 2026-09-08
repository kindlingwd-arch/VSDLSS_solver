/* VSDLSS function (decompiled by Ghidra) */
/* name: addToDestVecsByCols_vsdlss  addr: 009ec360  size: ca */
#include "vsdlss_ref.h"

/* signature: void addToDestVecsByCols_vsdlss(int param_1,long param_2,int param_3,int param_4,long param_5,long param_6); */

void addToDestVecsByCols_vsdlss
               (int param_1,long param_2,int param_3,int param_4,long param_5,long param_6)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  
  if (param_1 == param_3) {
    if (0 < param_4) {
      plVar6 = (long *)(param_6 + 8);
      lVar5 = (ulong)(param_1 - 1) * 8 + 8;
      do {
        lVar3 = *plVar6;
        if (0 < param_1) {
          lVar4 = 0;
          do {
            *(double *)(lVar3 + 8 + lVar4) =
                 *(double *)(lVar3 + 8 + lVar4) + *(double *)(param_5 + 8 + lVar4);
            lVar4 = lVar4 + 8;
          } while (lVar4 != lVar5);
          param_5 = param_5 + lVar5;
        }
        plVar6 = plVar6 + 1;
      } while (plVar6 != (long *)(param_6 + 0x10 + (ulong)(param_4 - 1) * 8));
      return;
    }
  }
  else {
    if (param_4 < 1) {
      return;
    }
    plVar6 = (long *)(param_6 + 8);
    lVar5 = (ulong)(param_1 - 1) + 1;
    do {
      lVar3 = *plVar6;
      if (0 < param_1) {
        lVar4 = 0;
        do {
          pdVar1 = (double *)(lVar3 + (long)*(int *)(param_2 + 4 + lVar4) * 8);
          lVar2 = lVar4 * 2;
          lVar4 = lVar4 + 4;
          *pdVar1 = *pdVar1 + *(double *)(param_5 + 8 + lVar2);
        } while (lVar4 != lVar5 * 4);
        param_5 = param_5 + lVar5 * 8;
      }
      plVar6 = plVar6 + 1;
    } while (plVar6 != (long *)(param_6 + 0x10 + (ulong)(param_4 - 1) * 8));
  }
  return;
}


