/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindReachArr_vsdlss  addr: 00a28750  size: 1ee */
#include "vsdlss_ref.h"

/* signature: void MLDFindReachArr_vsdlss(int param_1,long param_2,long param_3,int param_4,long param_5,int *param_6); */

void MLDFindReachArr_vsdlss
               (int param_1,long param_2,long param_3,int param_4,long param_5,int *param_6)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int *piVar10;
  long lVar11;
  int *piVar12;
  int iVar13;
  int local_74;
  
  lVar11 = (long)param_1;
  lVar6 = *(long *)(param_3 + 0x28);
  lVar7 = *(long *)(param_6 + 2);
  lVar8 = *(long *)(param_3 + 0x38);
  iVar2 = *param_6;
  piVar12 = *(int **)(lVar8 + lVar11 * 8);
  iVar3 = *(int *)(lVar6 + lVar11 * 4);
  *(int *)(lVar7 + lVar11 * 4) = iVar2;
  if (iVar3 < 1) {
    iVar13 = 0;
  }
  else {
    iVar13 = 0;
    local_74 = 1;
    do {
      piVar12 = piVar12 + 1;
      iVar4 = *piVar12;
      lVar11 = (long)iVar4;
      iVar9 = *(int *)(param_2 + lVar11 * 4);
      if (iVar9 < -0x3ffffff9) {
        piVar10 = (int *)(lVar11 * 4 + lVar7);
        if (*piVar10 < iVar2) {
          *piVar10 = iVar2;
          iVar13 = iVar13 + 1;
          *(int *)(param_5 + (long)iVar13 * 4) = iVar4;
        }
      }
      else if (iVar9 + 0x3ffffff9U < 0x3ffffff9) {
        iVar4 = *(int *)(lVar6 + lVar11 * 4);
        if (0 < iVar4) {
          piVar10 = (int *)(*(long *)(lVar8 + lVar11 * 8) + 4);
          iVar9 = 1;
          do {
            while( true ) {
              iVar5 = *piVar10;
              if (*(int *)(param_2 + (long)iVar5 * 4) < -0x3ffffff9) break;
              iVar9 = iVar9 + 1;
              piVar10 = piVar10 + 1;
              nrerror_vsdlss(1,"mldmmd.c",0x1aa);
              if (iVar4 < iVar9) goto LAB_00a287da;
            }
            piVar1 = (int *)(lVar7 + (long)iVar5 * 4);
            if (*piVar1 < iVar2) {
              iVar13 = iVar13 + 1;
              *piVar1 = iVar2;
              *(int *)(param_5 + (long)iVar13 * 4) = iVar5;
            }
            iVar9 = iVar9 + 1;
            piVar10 = piVar10 + 1;
          } while (iVar9 <= iVar4);
        }
      }
      else {
        nrerror_vsdlss(1,"mldmmd.c",0x1b1);
      }
LAB_00a287da:
      local_74 = local_74 + 1;
    } while (local_74 <= iVar3);
  }
  unmarkCountingMark_vsdlss(param_6);
  if (iVar13 == param_4) {
    return;
  }
  nrerror_vsdlss(1,"mldmmd.c",0x1ba);
  return;
}


