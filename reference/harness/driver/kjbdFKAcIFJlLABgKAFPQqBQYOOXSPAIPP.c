/* driver: kjbdFKAcIFJlLABgKAFPQqBQYOOXSPAIPP addr=009b4050 size=161 */
#include "vsdlss_ref.h"


long kjbdFKAcIFJlLABgKAFPQqBQYOOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long *param_6,
               long param_7)

{
  long *plVar1;
  long *plVar2;
  long *plVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long *plVar11;
  long lVar12;
  long lVar13;
  long local_50;
  
  lVar4 = *(long *)(param_4 + 0x50);
  lVar5 = *(long *)(param_4 + 0x60);
  lVar6 = param_6[2];
  plVar1 = (long *)(lVar4 + param_1 * 8);
  lVar7 = *param_6;
  lVar8 = *plVar1;
  plVar11 = (long *)(*(long *)(lVar5 + param_1 * 8) + 8);
  plVar2 = plVar11 + lVar8;
  if (0 < lVar8) {
    do {
      lVar9 = *plVar11;
      plVar11 = plVar11 + 1;
      *(long *)(lVar6 + lVar9 * 8) = lVar7;
    } while (plVar11 != plVar2);
  }
  if (param_2 < 1) {
    local_50 = 1;
  }
  else {
    local_50 = 0;
    plVar11 = (long *)(param_3 + 8);
    plVar2 = plVar11 + param_2;
    do {
      lVar9 = *plVar11;
      if ((*(long *)(lVar4 + lVar9 * 8) == lVar8) &&
         (((param_7 == 0 || (*(char *)(param_7 + lVar9) == '\0')) &&
          (*(long *)(param_5 + lVar9 * 8) < 1)))) {
        if (lVar8 < 1) {
          lVar13 = 0;
        }
        else {
          lVar12 = 1;
          lVar13 = 0;
          do {
            lVar10 = *(long *)(*(long *)(lVar5 + lVar9 * 8) + lVar12 * 8);
            if ((lVar10 != param_1) && (lVar13 = lVar13 + 1, *(long *)(lVar6 + lVar10 * 8) < lVar7))
            goto LAB_009b40d8;
            lVar12 = lVar12 + 1;
          } while (lVar12 <= lVar8);
        }
        if (*plVar1 - (ulong)(lVar7 <= *(long *)(lVar6 + lVar9 * 8)) == lVar13) {
          local_50 = local_50 + 1;
          plVar3 = (long *)(param_3 + local_50 * 8);
          lVar13 = *plVar3;
          *plVar3 = lVar9;
          *plVar11 = lVar13;
        }
      }
LAB_009b40d8:
      plVar11 = plVar11 + 1;
    } while (plVar11 != plVar2);
    local_50 = local_50 + 1;
  }
  RKJyOHaLRKQFKDkyOHXSPAIPP(param_6);
  return local_50;
}


