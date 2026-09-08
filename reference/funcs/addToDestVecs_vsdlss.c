/* VSDLSS function (decompiled by Ghidra) */
/* name: addToDestVecs_vsdlss  addr: 009ec430  size: c0 */
#include "vsdlss_ref.h"

/* signature: void addToDestVecs_vsdlss(int param_1,long param_2,int param_3,int param_4,long param_5,long param_6); */

void addToDestVecs_vsdlss
               (int param_1,long param_2,int param_3,int param_4,long param_5,long param_6)

{
  long lVar1;
  double *pdVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  double *pdVar6;
  int *piVar7;
  
  if (param_1 == param_3) {
    if (0 < param_1) {
      lVar4 = 8;
      lVar1 = (ulong)(param_4 - 1) * 8 + 8;
      do {
        if (0 < param_4) {
          lVar5 = 0;
          do {
            pdVar6 = (double *)(lVar4 + *(long *)(param_6 + 8 + lVar5));
            pdVar2 = (double *)(param_5 + 8 + lVar5);
            lVar5 = lVar5 + 8;
            *pdVar6 = *pdVar6 + *pdVar2;
          } while (lVar5 != lVar1);
          param_5 = param_5 + lVar1;
        }
        lVar4 = lVar4 + 8;
      } while (lVar4 != (ulong)(param_1 - 1) * 8 + 0x10);
      return;
    }
  }
  else {
    if (param_1 < 1) {
      return;
    }
    piVar7 = (int *)(param_2 + 4);
    lVar1 = (ulong)(param_4 - 1) * 8 + 8;
    do {
      iVar3 = *piVar7;
      if (0 < param_4) {
        lVar4 = 0;
        do {
          pdVar6 = (double *)((long)iVar3 * 8 + *(long *)(param_6 + 8 + lVar4));
          pdVar2 = (double *)(param_5 + 8 + lVar4);
          lVar4 = lVar4 + 8;
          *pdVar6 = *pdVar6 + *pdVar2;
        } while (lVar4 != lVar1);
        param_5 = param_5 + lVar1;
      }
      piVar7 = piVar7 + 1;
    } while (piVar7 != (int *)(param_2 + 8 + (ulong)(param_1 - 1) * 4));
  }
  return;
}


