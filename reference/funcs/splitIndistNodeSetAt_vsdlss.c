/* VSDLSS function (decompiled by Ghidra) */
/* name: splitIndistNodeSetAt_vsdlss  addr: 00a21d90  size: f5 */
#include "vsdlss_ref.h"

/* signature: void splitIndistNodeSetAt_vsdlss(long param_1,int param_2,int param_3); */

void splitIndistNodeSetAt_vsdlss(long param_1,int param_2,int param_3)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  long lVar5;
  
  if (param_2 == param_3) {
    nrerror_vsdlss(1,"indistnodesetsutils.c",0x4b);
  }
  piVar2 = *(int **)(*(long *)(param_1 + 8) + (long)param_2 * 8);
  if ((piVar2 == (int *)0x0) || (*(long *)(*(long *)(param_1 + 8) + (long)param_3 * 8) != 0)) {
    nrerror_vsdlss(1,"indistnodesetsutils.c",0x55);
  }
  else if (*piVar2 != param_2) {
    nrerror_vsdlss(1,"indistnodesetsutils.c",0x5a);
  }
  piVar3 = *(int **)(piVar2 + 2);
  if (piVar3 != (int *)0x0) {
    iVar1 = *piVar3;
    while( true ) {
      if (iVar1 == param_3) {
        lVar5 = *(long *)(param_1 + 8);
        piVar2[2] = 0;
        piVar2[3] = 0;
        *(int **)(lVar5 + (long)param_3 * 8) = piVar3;
        return;
      }
      piVar4 = *(int **)(piVar3 + 2);
      if (piVar4 == (int *)0x0) break;
      iVar1 = *piVar4;
      piVar2 = piVar3;
      piVar3 = piVar4;
    }
  }
  nrerror_vsdlss(1,"indistnodesetsutils.c",0x6b);
  return;
}


