/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDRMMatching_vsdlss  addr: 009fda20  size: 175 */
#include "vsdlss_ref.h"

/* signature: int MLDRMMatching_vsdlss(int param_1,long param_2,long param_3,long param_4); */

int MLDRMMatching_vsdlss(int param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int *piVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  long lVar14;
  int iVar15;
  long lVar16;
  int iVar17;
  int local_54;
  
  iVar4 = *(int *)(param_2 + 8);
  lVar6 = *(long *)(param_2 + 0x58);
  lVar7 = *(long *)(param_2 + 0x28);
  lVar8 = *(long *)(param_2 + 0x60);
  initIVector_vsdlss(param_3,(long)iVar4,0xffffffff);
  if (iVar4 < 1) {
    local_54 = 0;
  }
  else {
    piVar13 = (int *)(param_4 + 4);
    local_54 = 0;
    piVar2 = (int *)(param_4 + 8 + (ulong)(iVar4 - 1) * 4);
    do {
      while( true ) {
        iVar4 = *piVar13;
        lVar16 = (long)iVar4;
        lVar1 = lVar16 * 4;
        piVar3 = (int *)(param_3 + lVar1);
        if (*piVar3 != -1) break;
        if (lVar6 == 0) {
          iVar17 = 1;
        }
        else {
          iVar17 = *(int *)(lVar6 + lVar16 * 4);
        }
        iVar5 = *(int *)(lVar7 + lVar16 * 4);
        piVar9 = *(int **)(*(long *)(param_2 + 0x38) + lVar16 * 8);
        piVar10 = piVar3;
        lVar14 = lVar1;
        iVar12 = iVar4;
        if (0 < iVar5) {
          iVar11 = 1;
          do {
            piVar9 = piVar9 + 1;
            iVar12 = *piVar9;
            if (lVar6 == 0) {
              iVar15 = 1;
              lVar14 = (long)iVar12 << 2;
            }
            else {
              iVar15 = *(int *)(lVar6 + (long)iVar12 * 4);
              lVar14 = (long)iVar12 * 4;
            }
          } while (((*(int *)(param_3 + lVar14) != -1) ||
                   (piVar10 = (int *)(param_3 + lVar14), param_1 < iVar15 + iVar17)) &&
                  (iVar11 = iVar11 + 1, piVar10 = piVar3, lVar14 = lVar1, iVar12 = iVar4,
                  iVar11 <= iVar5));
        }
        local_54 = local_54 + 1;
        piVar13 = piVar13 + 1;
        *(int *)(lVar8 + lVar16 * 4) = local_54;
        *(int *)(lVar8 + lVar14) = local_54;
        *piVar10 = iVar4;
        *piVar3 = iVar12;
        if (piVar13 == piVar2) {
          return local_54;
        }
      }
      piVar13 = piVar13 + 1;
    } while (piVar13 != piVar2);
  }
  return local_54;
}


