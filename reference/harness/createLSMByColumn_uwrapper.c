/* harness: createLSMByColumn_uwrapper addr=00a419a0 size=93 */
#include "vsdlss_ref.h"


void createLSMByColumn_uwrapper(int *param_1)

{
  undefined8 *puVar1;
  int *piVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  int *piVar7;
  undefined8 uVar8;
  int iVar9;
  undefined8 *local_40;
  
  iVar9 = *param_1;
  lVar4 = *(long *)(param_1 + 6);
  lVar5 = *(long *)(param_1 + 0x12);
  lVar6 = *(long *)(param_1 + 10);
  initIVector_uwrapper(lVar4,iVar9,0);
  local_40 = (undefined8 *)(lVar5 + (long)iVar9 * 8);
  if (0 < iVar9) {
    do {
      piVar7 = (int *)*local_40;
      while (piVar7 != (int *)0x0) {
        iVar3 = *piVar7;
        puVar1 = (undefined8 *)(lVar6 + (long)iVar3 * 8);
        uVar8 = newFEdgelist_uwrapper(*(undefined8 *)(piVar7 + 2),iVar9,*puVar1);
        *puVar1 = uVar8;
        piVar7 = *(int **)(piVar7 + 4);
        piVar2 = (int *)(lVar4 + (long)iVar3 * 4);
        *piVar2 = *piVar2 + 1;
      }
      local_40 = local_40 + -1;
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
  }
  return;
}


