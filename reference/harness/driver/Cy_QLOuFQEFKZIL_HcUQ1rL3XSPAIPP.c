/* driver: Cy_QLOuFQEFKZIL_HcUQ1rL3XSPAIPP addr=00981de0 size=222 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void Cy_QLOuFQEFKZIL_HcUQ1rL3XSPAIPP(long param_1,long param_2,long param_3)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  ulong uVar8;
  long lVar9;
  double dVar10;
  double dVar11;
  
  lVar3 = *(long *)(param_3 + 0x58);
  lVar4 = *(long *)(*(long *)(param_3 + 0x48) + param_1 * 8);
  if (2 < param_2 - 1U) {
    KOBOOLOXSPAIPP(1,"factorapar_ls.c",0x39);
  }
  lVar5 = *(long *)(lVar3 + param_1 * 8);
  if (*(double *)(lVar5 + 8) <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x3f,param_1);
  }
  uVar8 = DAT_00abd200;
  dVar7 = _DAT_00ab67f8;
  if (1 < param_2) {
    lVar6 = *(long *)(lVar3 + 8 + param_1 * 8);
    lVar9 = 1;
    dVar11 = _DAT_00ab67f8 / *(double *)(lVar5 + 8);
    dVar10 = (double)(*(ulong *)(lVar5 + 0x10) ^ DAT_00abd200);
    if (1 < lVar4) {
      do {
        *(double *)(lVar6 + lVar9 * 8) =
             *(double *)(lVar5 + 8 + lVar9 * 8) * dVar10 * dVar11 + *(double *)(lVar6 + lVar9 * 8);
        lVar9 = lVar9 + 1;
      } while (lVar9 != lVar4);
    }
    if (*(double *)(lVar6 + 8) <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x4e,param_1 + 1);
    }
    if (param_2 != 2) {
      dVar10 = *(double *)(lVar6 + 8);
      lVar3 = *(long *)(lVar3 + 0x10 + param_1 * 8);
      lVar9 = 1;
      uVar1 = *(ulong *)(lVar5 + 0x18);
      uVar2 = *(ulong *)(lVar6 + 0x10);
      if (2 < lVar4) {
        do {
          *(double *)(lVar3 + lVar9 * 8) =
               *(double *)(lVar5 + 0x10 + lVar9 * 8) * (double)(uVar1 ^ uVar8) * dVar11 +
               *(double *)(lVar6 + 8 + lVar9 * 8) * (double)(uVar2 ^ uVar8) * (dVar7 / dVar10) +
               *(double *)(lVar3 + lVar9 * 8);
          lVar9 = lVar9 + 1;
        } while (lVar9 != lVar4 + -1);
      }
      if (*(double *)(lVar3 + 8) <= 0.0) {
        KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x5f,param_1 + 2);
        return;
      }
    }
  }
  return;
}


