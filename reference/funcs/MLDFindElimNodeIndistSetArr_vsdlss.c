/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindElimNodeIndistSetArr_vsdlss  addr: 00a28de0  size: 159 */
#include "vsdlss_ref.h"

/* signature: int MLDFindElimNodeIndistSetArr_vsdlss(int param_1,int param_2,long param_3,long param_4,long param_5,int *param_6,long param_7); */

int MLDFindElimNodeIndistSetArr_vsdlss
              (int param_1,int param_2,long param_3,long param_4,long param_5,int *param_6,
              long param_7)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  int *piVar7;
  long lVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int local_40;
  
  lVar4 = *(long *)(param_4 + 0x28);
  lVar5 = *(long *)(param_6 + 2);
  iVar2 = *param_6;
  piVar1 = (int *)(lVar4 + (long)param_1 * 4);
  iVar3 = *piVar1;
  lVar6 = *(long *)(param_4 + 0x38);
  lVar8 = *(long *)(lVar6 + (long)param_1 * 8);
  if (0 < iVar3) {
    piVar7 = (int *)(lVar8 + 4);
    do {
      iVar11 = *piVar7;
      piVar7 = piVar7 + 1;
      *(int *)(lVar5 + (long)iVar11 * 4) = iVar2;
    } while (piVar7 != (int *)(lVar8 + 8 + (ulong)(iVar3 - 1) * 4));
  }
  local_40 = 1;
  if (0 < param_2) {
    piVar7 = (int *)(param_3 + 4);
    local_40 = 0;
    do {
      lVar8 = (long)*piVar7;
      if ((*(int *)(lVar4 + lVar8 * 4) == iVar3) &&
         (((param_7 == 0 || (*(char *)(param_7 + lVar8) == '\0')) &&
          (*(int *)(param_5 + lVar8 * 4) < 1)))) {
        piVar9 = *(int **)(lVar6 + lVar8 * 8);
        if (iVar3 < 1) {
          iVar11 = 0;
        }
        else {
          iVar10 = 1;
          iVar11 = 0;
          do {
            piVar9 = piVar9 + 1;
            if ((*piVar9 != param_1) &&
               (iVar11 = iVar11 + 1, *(int *)(lVar5 + (long)*piVar9 * 4) < iVar2))
            goto LAB_00a28e70;
            iVar10 = iVar10 + 1;
          } while (iVar10 <= iVar3);
        }
        if (*piVar1 - (uint)(iVar2 <= *(int *)(lVar5 + lVar8 * 4)) == iVar11) {
          local_40 = local_40 + 1;
          piVar9 = (int *)(param_3 + (long)local_40 * 4);
          iVar11 = *piVar9;
          *piVar9 = *piVar7;
          *piVar7 = iVar11;
        }
      }
LAB_00a28e70:
      piVar7 = piVar7 + 1;
    } while (piVar7 != (int *)(param_3 + 8 + (ulong)(param_2 - 1) * 4));
    local_40 = local_40 + 1;
  }
  unmarkCountingMark_vsdlss(param_6);
  return local_40;
}


