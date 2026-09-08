/* driver: DBQdy_QLOqQyQPmKBaERKHXSPAIPP addr=00969bd0 size=1d6 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DBQdy_QLOqQyQPmKBaERKHXSPAIPP
               (long param_1,double *param_2,double *param_3,double *param_4,double *param_5,
               double *param_6,double *param_7,double *param_8,double *param_9)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long *plVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  
  lVar2 = *(long *)(param_1 + 0x28);
  lVar3 = *(long *)(param_1 + 0x10);
  lVar7 = *(long *)(param_1 + 0x40);
  lVar4 = *(long *)(param_1 + 0x48);
  *param_4 = 0.0;
  *param_7 = 0.0;
  *param_8 = 0.0;
  dVar10 = DAT_00ab7228;
  dVar11 = _DAT_00ab67f8;
  if (0 < lVar2) {
    plVar8 = (long *)(lVar7 + 8);
    lVar7 = 1;
    plVar1 = plVar8 + lVar2;
    do {
      while (lVar5 = *plVar8, lVar5 != 1) {
        dVar12 = (double)lVar5;
        dVar13 = (double)*(long *)(lVar4 + lVar7 * 8);
        *param_7 = (dVar13 * dVar12 - (dVar12 - dVar11) * dVar12 * dVar10) + *param_7;
        *param_4 = *param_4 + dVar13;
        if (0 < lVar5) {
          dVar12 = *param_8;
          lVar6 = 1;
          do {
            dVar9 = (double)lVar6;
            lVar6 = lVar6 + 1;
            dVar12 = dVar12 + ((dVar13 - dVar9) + dVar11) * (dVar13 - dVar9);
          } while (lVar6 != lVar5 + 1);
          *param_8 = dVar12;
        }
        plVar8 = plVar8 + 1;
        lVar7 = lVar7 + lVar5;
        if (plVar8 == plVar1) goto LAB_00969d24;
      }
      dVar12 = (double)*(long *)(lVar4 + lVar7 * 8);
      plVar8 = plVar8 + 1;
      lVar7 = lVar7 + 1;
      *param_7 = *param_7 + dVar12;
      *param_4 = *param_4 + dVar12;
      *param_8 = (dVar12 - dVar11) * dVar12 + *param_8;
    } while (plVar8 != plVar1);
  }
LAB_00969d24:
  dVar11 = DAT_00ab6798;
  dVar12 = (double)lVar3 * DAT_00ab6798;
  *param_4 = *param_4 * DAT_00ab6798;
  dVar10 = *param_7 * dVar11;
  *param_5 = dVar10;
  *param_6 = dVar10 + *param_4 + (double)lVar2 * dVar11 + dVar12 + dVar12 + dVar12;
  dVar11 = (*param_7 + *param_7) - (double)lVar3;
  *param_9 = dVar11 + dVar11;
  *param_2 = *param_6;
  *param_3 = *param_6;
  return;
}


