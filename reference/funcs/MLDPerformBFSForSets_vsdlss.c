/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDPerformBFSForSets_vsdlss  addr: 00a01320  size: 298 */
#include "vsdlss_ref.h"

/* signature: void MLDPerformBFSForSets_vsdlss(int param_1,long param_2,long param_3,long param_4,int *param_5,long param_6); */

void MLDPerformBFSForSets_vsdlss
               (int param_1,long param_2,long param_3,long param_4,int *param_5,long param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  long lVar14;
  int iVar15;
  long local_58;
  
  lVar4 = *(long *)(param_3 + 0x28);
  lVar5 = *(long *)(param_6 + 0x10);
  iVar1 = *(int *)(param_6 + 8);
  if (param_1 == 8) {
    lVar7 = 0;
    iVar8 = 1;
    if (*(int *)(param_2 + 4) < 1) {
      return;
    }
    do {
      iVar9 = (int)lVar7;
      if (*(int *)(param_4 + 4 + lVar7 * 4) == 0) {
        param_5[lVar7 + 1] = 1;
        if (iVar1 < iVar8) {
          nrerror_vsdlss(1,"mldorder.c",0xcbb);
        }
        lVar14 = (long)iVar8;
        iVar8 = iVar8 + 1;
        *(int *)(lVar5 + lVar14 * 4) = iVar9 + 1;
      }
      lVar7 = lVar7 + 1;
    } while (iVar9 + 2 <= *(int *)(param_2 + 4));
    iVar9 = 4;
    iVar15 = 1;
  }
  else {
    iVar9 = *(int *)(param_2 + 4) + 1;
    if (*(int *)(param_2 + 8) < iVar9) {
      return;
    }
    lVar7 = (long)iVar9;
    lVar14 = 0;
    iVar8 = 1;
    do {
      if (*(int *)(param_4 + lVar7 * 4 + lVar14) == 0) {
        *(undefined4 *)((long)param_5 + lVar14 + lVar7 * 4) = 6;
        if (iVar1 < iVar8) {
          nrerror_vsdlss(1,"mldorder.c",0xcca);
        }
        lVar6 = (long)iVar8;
        iVar8 = iVar8 + 1;
        *(int *)(lVar5 + lVar6 * 4) = iVar9;
      }
      iVar9 = iVar9 + 1;
      lVar14 = lVar14 + 4;
    } while (iVar9 <= *(int *)(param_2 + 8));
    iVar9 = 3;
    iVar15 = 6;
  }
  if (iVar8 != 1) {
    local_58 = 0;
    do {
      iVar11 = (int)local_58;
      lVar7 = (long)*(int *)(lVar5 + 4 + local_58 * 4);
      iVar2 = *(int *)(lVar4 + lVar7 * 4);
      piVar13 = *(int **)(*(long *)(param_3 + 0x38) + lVar7 * 8);
      if (0 < iVar2) {
        iVar10 = 1;
        do {
          piVar13 = piVar13 + 1;
          iVar3 = *piVar13;
          if (param_5[iVar3] != iVar9) {
            param_5[iVar3] = iVar9;
            iVar3 = *(int *)(param_4 + (long)iVar3 * 4);
            piVar12 = param_5;
            if ((iVar3 == 0) || (piVar12 = param_5 + iVar3, iVar15 == *piVar12)) {
              nrerror_vsdlss(1,"mldorder.c",0xcdf);
              *piVar12 = iVar15;
            }
            else {
              *piVar12 = iVar15;
            }
            if (iVar1 < iVar8) {
              nrerror_vsdlss(1,"mldorder.c",0xce6);
            }
            lVar7 = (long)iVar8;
            iVar8 = iVar8 + 1;
            *(int *)(lVar5 + lVar7 * 4) = iVar3;
          }
          iVar10 = iVar10 + 1;
        } while (iVar10 <= iVar2);
      }
      local_58 = local_58 + 1;
    } while (iVar11 + 2 < iVar8);
  }
  return;
}


