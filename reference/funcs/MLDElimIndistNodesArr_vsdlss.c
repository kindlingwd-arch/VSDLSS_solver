/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDElimIndistNodesArr_vsdlss  addr: 00a28f50  size: 619 */
#include "vsdlss_ref.h"

/* signature: void MLDElimIndistNodesArr_vsdlss(int param_1,int param_2,int param_3,long param_4,long param_5,long param_6,long param_7,int *param_8,int *param_9,int *param_10); */

void MLDElimIndistNodesArr_vsdlss
               (int param_1,int param_2,int param_3,long param_4,long param_5,long param_6,
               long param_7,int *param_8,int *param_9,int *param_10)

{
  long *plVar1;
  long lVar2;
  int *piVar3;
  uint *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  bool bVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  int *piVar14;
  long lVar15;
  int *piVar16;
  long lVar17;
  int *piVar18;
  int iVar19;
  long lVar20;
  int *piVar21;
  int local_40;
  
  lVar17 = *(long *)(param_6 + 0x38);
  local_40 = *param_9;
  lVar8 = *(long *)(param_6 + 0x28);
  iVar5 = *param_10;
  lVar9 = *(long *)(param_10 + 2);
  iVar6 = param_1;
  if (param_2 != 1) {
    iVar6 = *(int *)(param_4 + -4 + (long)param_2 * 4);
  }
  lVar20 = (long)param_1;
  *(int *)(lVar9 + lVar20 * 4) = iVar5;
  *(int *)(param_7 + lVar20 * 4) = iVar6;
  piVar18 = (int *)(lVar8 + lVar20 * 4);
  iVar11 = *piVar18;
  plVar1 = (long *)(lVar17 + lVar20 * 8);
  lVar15 = *plVar1;
  if (0 < iVar11) {
    piVar14 = (int *)(lVar15 + 4);
    do {
      iVar19 = *piVar14;
      lVar2 = (long)iVar19 * 4;
      piVar21 = (int *)(param_5 + lVar2);
      if ((*piVar21 + 0x3ffffff9U < 0x3ffffff9) &&
         (piVar16 = (int *)(lVar2 + lVar9), *piVar16 < iVar5)) {
        *piVar16 = iVar5;
        *(int *)(param_7 + (long)iVar19 * 4) = iVar6;
        *piVar21 = -*piVar21;
      }
      piVar14 = piVar14 + 1;
    } while (piVar14 != (int *)(lVar15 + 8 + (ulong)(iVar11 - 1) * 4));
  }
  if (param_2 < 2) {
    *(undefined4 *)(param_7 + (long)iVar6 * 4) = 0;
    local_40 = local_40 + 1;
    *(int *)(param_5 + lVar20 * 4) = local_40;
    *plVar1 = param_4;
    *piVar18 = param_3;
  }
  else {
    piVar14 = (int *)(param_4 + 4);
    piVar21 = piVar14;
    do {
      iVar11 = *piVar21;
      piVar21 = piVar21 + 1;
      *(int *)(lVar9 + (long)iVar11 * 4) = iVar5;
      *(int *)(param_7 + (long)iVar11 * 4) = iVar6;
    } while (piVar21 != (int *)(param_4 + 8 + (ulong)(param_2 - 2) * 4));
    iVar11 = local_40 + 1;
    *(undefined4 *)(param_7 + (long)iVar6 * 4) = 0;
    *(int *)(param_5 + lVar20 * 4) = iVar11;
    *plVar1 = param_4;
    *piVar18 = param_3;
    iVar19 = param_3;
    do {
      iVar19 = iVar19 + -1;
      lVar15 = (long)*piVar14;
      iVar11 = iVar11 + 1;
      *(int *)(param_5 + lVar15 * 4) = iVar11;
      *(int **)(lVar17 + lVar15 * 8) = piVar14;
      piVar14 = piVar14 + 1;
      *(int *)(lVar8 + lVar15 * 4) = iVar19;
    } while (iVar11 != param_2 + local_40);
    local_40 = local_40 + param_2;
  }
  piVar18 = (int *)(param_5 + (long)iVar6 * 4);
  *piVar18 = -*piVar18;
  if (param_3 < param_2) {
    unmarkCountingMark_vsdlss(param_10);
  }
  else {
    piVar18 = (int *)(param_4 + (long)param_2 * 4);
    piVar14 = piVar18;
    iVar11 = param_2;
    do {
      puVar4 = (uint *)(lVar9 + (long)*piVar14 * 4);
      uVar12 = *puVar4;
      if (iVar5 <= (int)uVar12) {
        nrerror_vsdlss(1,"mldmmd.c",0x341);
        uVar12 = *puVar4;
      }
      iVar11 = iVar11 + 1;
      piVar14 = piVar14 + 1;
      *puVar4 = ~uVar12;
    } while (iVar11 <= param_3);
    piVar14 = piVar18;
    iVar11 = param_2;
    do {
      while( true ) {
        lVar15 = (long)*piVar14;
        iVar19 = *(int *)(param_5 + lVar15 * 4);
        piVar21 = (int *)(lVar8 + lVar15 * 4);
        piVar16 = (int *)(lVar15 * 4 + param_7);
        iVar7 = *piVar21;
        iVar13 = *piVar16;
        if (iVar13 < 0) {
          nrerror_vsdlss(1,"mldmmd.c",0x34d);
          iVar13 = *piVar16;
        }
        if (((iVar7 < 0xccccccd) || ((double)iVar7 * 0.0 <= (double)(iVar13 + 1 + param_3))) ||
           (iVar19 != -0x7ffffff4)) break;
        *piVar16 = iVar13 + param_3 + 1;
LAB_00a291b7:
        iVar11 = iVar11 + 1;
        piVar14 = piVar14 + 1;
        if (param_3 < iVar11) goto LAB_00a29300;
      }
      if (0 < iVar13) {
        *piVar16 = -0x7ffffff5;
        goto LAB_00a291b7;
      }
      *piVar16 = 0;
      lVar15 = *(long *)(lVar17 + lVar15 * 8);
      if (iVar7 < 1) {
        *piVar21 = 0;
      }
      else {
        piVar16 = (int *)(lVar15 + 4);
        iVar19 = 0;
        bVar10 = true;
        piVar3 = (int *)(lVar15 + 8 + (ulong)(iVar7 - 1) * 4);
        do {
          while( true ) {
            iVar7 = *piVar16;
            iVar13 = *(int *)(lVar9 + (long)iVar7 * 4);
            if (iVar13 < iVar5) break;
            if (bVar10) {
              iVar19 = iVar19 + 1;
              bVar10 = false;
              *(int *)(lVar15 + (long)iVar19 * 4) = iVar6;
            }
LAB_00a29282:
            piVar16 = piVar16 + 1;
            if (piVar16 == piVar3) goto LAB_00a292b0;
          }
          if (iVar13 < 0) goto LAB_00a29282;
          iVar19 = iVar19 + 1;
          piVar16 = piVar16 + 1;
          *(int *)(lVar15 + (long)iVar19 * 4) = iVar7;
        } while (piVar16 != piVar3);
LAB_00a292b0:
        *piVar21 = iVar19;
        if (!bVar10) goto LAB_00a291b7;
      }
      iVar11 = iVar11 + 1;
      piVar14 = piVar14 + 1;
      nrerror_vsdlss(1,"mldmmd.c",0x378);
    } while (iVar11 <= param_3);
LAB_00a29300:
    piVar14 = piVar18;
    do {
      iVar5 = *piVar14;
      piVar14 = piVar14 + 1;
      puVar4 = (uint *)(lVar9 + (long)iVar5 * 4);
      *puVar4 = ~*puVar4;
    } while (piVar14 !=
             (int *)(param_4 + 4 + ((ulong)(uint)(param_3 - param_2) + (long)param_2) * 4));
    unmarkCountingMark_vsdlss(param_10);
    do {
      while( true ) {
        iVar5 = *piVar18;
        lVar17 = (long)iVar5;
        iVar6 = *(int *)(param_5 + lVar17 * 4);
        if (*(int *)(param_7 + lVar17 * 4) == -0x7ffffff5) break;
LAB_00a29360:
        param_2 = param_2 + 1;
        piVar18 = piVar18 + 1;
        if (param_3 < param_2) goto LAB_00a29430;
      }
      iVar19 = MLDUpdateNodeAdjArray_vsdlss(iVar5,param_5,param_6,param_7,param_10);
      iVar11 = *(int *)(*(long *)(param_8 + 8) + lVar17 * 4);
      if ((0 < iVar11) && (iVar11 <= *param_8 * 2)) {
        iVar19 = iVar19 + (-0x7ffffff3 - iVar6);
        remFromBucketBpq_vsdlss(iVar5,param_8);
        piVar21 = (int *)(lVar17 * 4 + *(long *)(param_8 + 10));
        piVar14 = (int *)(*(long *)(param_8 + 6) + (long)iVar19 * 4);
        *piVar21 = *piVar14;
        lVar8 = *(long *)(param_8 + 8);
        *(int *)(lVar8 + lVar17 * 4) = iVar19 + *param_8;
        *piVar14 = iVar5;
        iVar6 = *piVar21;
        if (param_8[4] != iVar6) {
          *(int *)(lVar8 + (long)iVar6 * 4) = iVar5;
        }
        param_8[2] = param_8[2] + 1;
        goto LAB_00a29360;
      }
      if (iVar11 != param_8[4]) goto LAB_00a29360;
      param_2 = param_2 + 1;
      nrerror_vsdlss(1,"mldmmd.c",0x392);
      piVar18 = piVar18 + 1;
    } while (param_2 <= param_3);
  }
LAB_00a29430:
  *param_9 = local_40;
  return;
}


