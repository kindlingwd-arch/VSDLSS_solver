/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFinishReordering_vsdlss  addr: 00a2ad50  size: dd */
#include "vsdlss_ref.h"

/* signature: void MLDFinishReordering_vsdlss(int param_1,int param_2,long param_3,int *param_4); */

void MLDFinishReordering_vsdlss(int param_1,int param_2,long param_3,int *param_4)

{
  int iVar1;
  long lVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  
  lVar2 = *(long *)(param_4 + 2);
  iVar1 = *param_4;
  if (param_1 != param_2) {
    nrerror_vsdlss(1,"mldmmd.c",0x719);
  }
  piVar5 = (int *)(param_3 + 4);
  iVar6 = 1;
  if (0 < param_1) {
    do {
      while( true ) {
        iVar4 = *piVar5;
        if (iVar4 < 0) {
          iVar4 = -iVar4;
          *piVar5 = iVar4;
        }
        if ((iVar4 <= param_1) && (0 < iVar4)) break;
        nrerror_vsdlss(1,"mldmmd.c",0x724);
        piVar3 = (int *)(lVar2 + (long)iVar4 * 4);
        if (*piVar3 < iVar1) goto LAB_00a2adb1;
LAB_00a2adf0:
        iVar6 = iVar6 + 1;
        piVar5 = piVar5 + 1;
        nrerror_vsdlss(1,"mldmmd.c",0x72d);
        if (param_1 < iVar6) goto LAB_00a2ae13;
      }
      piVar3 = (int *)(lVar2 + (long)iVar4 * 4);
      if (iVar1 <= *piVar3) goto LAB_00a2adf0;
LAB_00a2adb1:
      iVar6 = iVar6 + 1;
      piVar5 = piVar5 + 1;
      *piVar3 = iVar1;
    } while (iVar6 <= param_1);
  }
LAB_00a2ae13:
  unmarkCountingMark_vsdlss(param_4);
  return;
}


