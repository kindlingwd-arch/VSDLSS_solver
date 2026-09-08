/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindDegInElimGraphArr_vsdlss  addr: 00a283e0  size: 19c */
#include "vsdlss_ref.h"

/* signature: int MLDFindDegInElimGraphArr_vsdlss(int param_1,long param_2,long param_3,int *param_4); */

int MLDFindDegInElimGraphArr_vsdlss(int param_1,long param_2,long param_3,int *param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  int *piVar9;
  long lVar10;
  int *piVar11;
  int iVar12;
  int local_60;
  
  lVar10 = (long)param_1;
  lVar5 = *(long *)(param_3 + 0x38);
  lVar6 = *(long *)(param_4 + 2);
  lVar7 = *(long *)(param_3 + 0x28);
  iVar2 = *param_4;
  piVar11 = *(int **)(lVar5 + lVar10 * 8);
  iVar3 = *(int *)(lVar7 + lVar10 * 4);
  *(int *)(lVar6 + lVar10 * 4) = iVar2;
  if (iVar3 < 1) {
    iVar12 = 0;
  }
  else {
    local_60 = 1;
    iVar12 = 0;
    do {
      piVar11 = piVar11 + 1;
      lVar10 = (long)*piVar11;
      iVar4 = *(int *)(param_2 + lVar10 * 4);
      if (iVar4 < -0x3ffffff9) {
        piVar9 = (int *)(lVar10 * 4 + lVar6);
        if (*piVar9 < iVar2) {
          *piVar9 = iVar2;
          iVar12 = iVar12 + 1;
        }
      }
      else if (iVar4 + 0x3ffffff9U < 0x3ffffff9) {
        iVar4 = *(int *)(lVar7 + lVar10 * 4);
        if (0 < iVar4) {
          piVar9 = (int *)(*(long *)(lVar5 + lVar10 * 8) + 4);
          iVar8 = 1;
          do {
            while (*(int *)(param_2 + (long)*piVar9 * 4) < -0x3ffffff9) {
              piVar1 = (int *)(lVar6 + (long)*piVar9 * 4);
              if (*piVar1 < iVar2) {
                *piVar1 = iVar2;
                iVar12 = iVar12 + 1;
              }
              iVar8 = iVar8 + 1;
              piVar9 = piVar9 + 1;
              if (iVar4 < iVar8) goto LAB_00a2845f;
            }
            iVar8 = iVar8 + 1;
            piVar9 = piVar9 + 1;
            nrerror_vsdlss(1,"mldmmd.c",0x121);
          } while (iVar8 <= iVar4);
        }
      }
      else {
        nrerror_vsdlss(1,"mldmmd.c",0x128);
      }
LAB_00a2845f:
      local_60 = local_60 + 1;
    } while (local_60 <= iVar3);
  }
  unmarkCountingMark_vsdlss(param_4);
  return iVar12;
}


