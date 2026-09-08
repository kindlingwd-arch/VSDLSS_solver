/* VSDLSS function (decompiled by Ghidra) */
/* name: iterOrderDeg1Nodes_vsdlss  addr: 00a0f990  size: 302 */
#include "vsdlss_ref.h"

/* signature: void iterOrderDeg1Nodes_vsdlss(long param_1,int *param_2,int *param_3,long param_4,long param_5,long param_6); */

void iterOrderDeg1Nodes_vsdlss
               (long param_1,int *param_2,int *param_3,long param_4,long param_5,long param_6)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  int iVar14;
  uint uVar15;
  int *piVar16;
  int iVar17;
  long lVar18;
  int *piVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  
  iVar2 = *(int *)(param_1 + 4);
  lVar7 = *(long *)(param_1 + 0x10);
  lVar8 = *(long *)(param_1 + 8);
  iVar20 = *param_2;
  lVar11 = newIVector_vsdlss((long)iVar2,0,0);
  if (iVar2 < 1) {
    iVar14 = 0;
  }
  else {
    lVar18 = 0;
    iVar14 = 0;
    iVar21 = 1;
    iVar22 = 1;
    do {
      while ((iVar17 = (int)lVar18, *(char *)(param_6 + 1 + lVar18) != '\0' ||
             (uVar15 = *(uint *)(param_5 + 4 + lVar18 * 4), 1 < uVar15))) {
        lVar18 = lVar18 + 1;
        iVar22 = iVar17 + 2;
        if (iVar2 < iVar22) goto LAB_00a0fa66;
      }
      iVar14 = iVar14 + (uint)(uVar15 == 0);
      if (*(int *)(lVar7 + 4 + lVar18 * 4) == 1) {
        iVar14 = iVar14 + (uint)(*(int *)(param_5 +
                                         (long)*(int *)(*(long *)(lVar8 + 8 + lVar18 * 8) + 4) * 4)
                                == -1);
      }
      if (iVar2 < iVar21) {
        nrerror_vsdlss(1,"reduceproblem.c",0xd0);
      }
      lVar12 = (long)iVar21;
      iVar21 = iVar21 + 1;
      *(int *)(lVar11 + lVar12 * 4) = iVar22;
      *(undefined4 *)(param_5 + 4 + lVar18 * 4) = 0xffffffff;
      lVar18 = lVar18 + 1;
      iVar22 = iVar17 + 2;
    } while (iVar22 <= iVar2);
LAB_00a0fa66:
    if (iVar21 != 1) {
      lVar18 = (long)iVar20;
      lVar12 = 0;
      iVar22 = iVar20 + 1;
      do {
        iVar9 = (int)lVar12;
        iVar17 = *(int *)(lVar11 + 4 + lVar12 * 4);
        *(int *)(param_4 + lVar18 * 4 + 4 + lVar12 * 4) = iVar17;
        iVar3 = *(int *)(lVar7 + (long)iVar17 * 4);
        iVar20 = iVar22 + iVar9;
        if (0 < iVar3) {
          piVar19 = (int *)(*(long *)(lVar8 + (long)iVar17 * 8) + 4);
          iVar17 = 1;
          do {
            while( true ) {
              iVar4 = *piVar19;
              lVar13 = (long)iVar4;
              puVar1 = (uint *)(param_5 + lVar13 * 4);
              if (*puVar1 != 0xffffffff) break;
LAB_00a0fad0:
              iVar17 = iVar17 + 1;
              piVar19 = piVar19 + 1;
              if (iVar3 < iVar17) goto LAB_00a0fb78;
            }
            uVar15 = *puVar1 - 1;
            *puVar1 = uVar15;
            if ((*(char *)(param_6 + lVar13) != '\0') || (1 < uVar15)) goto LAB_00a0fad0;
            iVar5 = *(int *)(lVar7 + lVar13 * 4);
            lVar13 = *(long *)(lVar8 + lVar13 * 8);
            if (iVar5 < 1) {
LAB_00a0fb46:
              iVar14 = iVar14 + 1;
            }
            else if (*(int *)(param_5 + (long)*(int *)(lVar13 + 4) * 4) == -1) {
              piVar16 = (int *)(lVar13 + 8);
              iVar10 = 1;
              do {
                iVar10 = iVar10 + 1;
                if (iVar5 < iVar10) goto LAB_00a0fb46;
                iVar6 = *piVar16;
                piVar16 = piVar16 + 1;
              } while (*(int *)(param_5 + (long)iVar6 * 4) == -1);
            }
            if (iVar2 < iVar21) {
              nrerror_vsdlss(1,"reduceproblem.c",0xf9);
            }
            iVar17 = iVar17 + 1;
            lVar13 = (long)iVar21;
            piVar19 = piVar19 + 1;
            iVar21 = iVar21 + 1;
            *(int *)(lVar11 + lVar13 * 4) = iVar4;
            *puVar1 = 0xffffffff;
          } while (iVar17 <= iVar3);
        }
LAB_00a0fb78:
        lVar12 = lVar12 + 1;
      } while (iVar9 + 2 != iVar21);
    }
  }
  if (lVar11 != 0) {
    casiFree_vsdlss(lVar11);
  }
  removeElimNodesFromBuckets(param_5,param_1);
  *param_2 = iVar20;
  *param_3 = iVar14;
  return;
}


