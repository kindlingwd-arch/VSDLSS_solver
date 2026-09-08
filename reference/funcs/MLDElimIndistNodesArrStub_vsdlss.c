/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDElimIndistNodesArrStub_vsdlss  addr: 00a29b60  size: 46e */
#include "vsdlss_ref.h"

/* signature: void MLDElimIndistNodesArrStub_vsdlss(int param_1,int param_2,int param_3,long param_4,long param_5,long param_6,long param_7,int *param_8,int *param_9); */

void MLDElimIndistNodesArrStub_vsdlss
               (int param_1,int param_2,int param_3,long param_4,long param_5,long param_6,
               long param_7,int *param_8,int *param_9)

{
  long *plVar1;
  long lVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  long lVar14;
  int *piVar15;
  int *piVar16;
  int iVar17;
  long lVar18;
  int local_84;
  int local_40;
  
  lVar6 = *(long *)(param_6 + 0x38);
  local_40 = *param_8;
  lVar7 = *(long *)(param_6 + 0x28);
  lVar8 = *(long *)(param_9 + 2);
  iVar4 = *param_9;
  local_84 = param_1;
  if (param_2 != 1) {
    local_84 = *(int *)(param_4 + -4 + (long)param_2 * 4);
  }
  lVar18 = (long)param_1;
  *(int *)(lVar8 + lVar18 * 4) = iVar4;
  *(int *)(param_7 + lVar18 * 4) = local_84;
  piVar16 = (int *)(lVar7 + lVar18 * 4);
  iVar10 = *piVar16;
  plVar1 = (long *)(lVar6 + lVar18 * 8);
  lVar14 = *plVar1;
  if (0 < iVar10) {
    piVar13 = (int *)(lVar14 + 4);
    do {
      iVar17 = *piVar13;
      lVar2 = (long)iVar17 * 4;
      piVar12 = (int *)(param_5 + lVar2);
      if ((*piVar12 + 0x3ffffff9U < 0x3ffffff9) &&
         (piVar15 = (int *)(lVar2 + lVar8), *piVar15 < iVar4)) {
        *piVar15 = iVar4;
        *(int *)(param_7 + (long)iVar17 * 4) = local_84;
        *piVar12 = -*piVar12;
      }
      piVar13 = piVar13 + 1;
    } while (piVar13 != (int *)(lVar14 + 8 + (ulong)(iVar10 - 1) * 4));
  }
  if (param_2 < 2) {
    *(undefined4 *)(param_7 + (long)local_84 * 4) = 0;
    local_40 = local_40 + 1;
    *(int *)(param_5 + lVar18 * 4) = local_40;
    *plVar1 = param_4;
    *piVar16 = param_3;
  }
  else {
    piVar13 = (int *)(param_4 + 4);
    piVar12 = piVar13;
    do {
      iVar10 = *piVar12;
      piVar12 = piVar12 + 1;
      *(int *)(lVar8 + (long)iVar10 * 4) = iVar4;
      *(int *)(param_7 + (long)iVar10 * 4) = local_84;
    } while (piVar12 != (int *)(param_4 + 8 + (ulong)(param_2 - 2) * 4));
    iVar10 = local_40 + 1;
    *(undefined4 *)(param_7 + (long)local_84 * 4) = 0;
    *(int *)(param_5 + lVar18 * 4) = iVar10;
    *plVar1 = param_4;
    *piVar16 = param_3;
    iVar17 = param_3;
    do {
      iVar17 = iVar17 + -1;
      lVar14 = (long)*piVar13;
      iVar10 = iVar10 + 1;
      *(int *)(param_5 + lVar14 * 4) = iVar10;
      *(int **)(lVar6 + lVar14 * 8) = piVar13;
      piVar13 = piVar13 + 1;
      *(int *)(lVar7 + lVar14 * 4) = iVar17;
    } while (iVar10 != param_2 + local_40);
    local_40 = local_40 + param_2;
  }
  piVar16 = (int *)(param_5 + (long)local_84 * 4);
  *piVar16 = -*piVar16;
  if (param_3 < param_2) {
    unmarkCountingMark_vsdlss(param_9);
  }
  else {
    piVar16 = (int *)(param_4 + (long)param_2 * 4);
    piVar13 = piVar16;
    do {
      iVar10 = *piVar13;
      piVar13 = piVar13 + 1;
      *(int *)(lVar8 + (long)iVar10 * 4) = iVar4;
    } while (piVar13 !=
             (int *)(param_4 + 4 + ((long)param_2 + (ulong)(uint)(param_3 - param_2)) * 4));
    piVar13 = piVar16;
    iVar10 = param_2;
    do {
      while( true ) {
        lVar14 = (long)*piVar13;
        iVar17 = *(int *)(param_5 + lVar14 * 4);
        piVar12 = (int *)(lVar7 + lVar14 * 4);
        piVar15 = (int *)(lVar14 * 4 + param_7);
        iVar5 = *piVar12;
        iVar11 = *piVar15;
        if (iVar11 < 0) {
          nrerror_vsdlss(1,"mldmmd.c",0x4c6);
          iVar11 = *piVar15;
        }
        if (((iVar5 < 0x81) || ((double)iVar5 <= (double)(iVar11 + 1 + param_3))) ||
           (iVar17 != -0x7ffffff4)) break;
        *piVar15 = iVar11 + param_3 + 1;
LAB_00a29d67:
        iVar10 = iVar10 + 1;
        piVar13 = piVar13 + 1;
        if (param_3 < iVar10) goto LAB_00a29ea0;
      }
      if (0 < iVar11) {
        *piVar15 = -0x7ffffff5;
        goto LAB_00a29d67;
      }
      *piVar15 = 0;
      lVar14 = *(long *)(lVar6 + lVar14 * 8);
      if (iVar5 < 1) {
        *piVar12 = 0;
      }
      else {
        piVar15 = (int *)(lVar14 + 4);
        iVar17 = 0;
        bVar9 = true;
        piVar3 = (int *)(lVar14 + 8 + (ulong)(iVar5 - 1) * 4);
        do {
          while (iVar5 = *piVar15, *(int *)(lVar8 + (long)iVar5 * 4) < iVar4) {
            iVar17 = iVar17 + 1;
            piVar15 = piVar15 + 1;
            *(int *)(lVar14 + (long)iVar17 * 4) = iVar5;
            if (piVar15 == piVar3) goto LAB_00a29e4a;
          }
          if (bVar9) {
            iVar17 = iVar17 + 1;
            bVar9 = false;
            *(int *)(lVar14 + (long)iVar17 * 4) = local_84;
          }
          piVar15 = piVar15 + 1;
        } while (piVar15 != piVar3);
LAB_00a29e4a:
        *piVar12 = iVar17;
        if (!bVar9) goto LAB_00a29d67;
      }
      iVar10 = iVar10 + 1;
      piVar13 = piVar13 + 1;
      nrerror_vsdlss(1,"mldmmd.c",0x4ef);
    } while (iVar10 <= param_3);
LAB_00a29ea0:
    unmarkCountingMark_vsdlss(param_9);
    do {
      while (iVar4 = *piVar16, *(int *)(param_7 + (long)iVar4 * 4) != -0x7ffffff5) {
        param_2 = param_2 + 1;
        piVar16 = piVar16 + 1;
        if (param_3 < param_2) goto LAB_00a29f10;
      }
      param_2 = param_2 + 1;
      piVar16 = piVar16 + 1;
      MLDUpdateNodeAdjArray_vsdlss(iVar4,param_5,param_6,param_7,param_9);
    } while (param_2 <= param_3);
  }
LAB_00a29f10:
  *param_8 = local_40;
  return;
}


