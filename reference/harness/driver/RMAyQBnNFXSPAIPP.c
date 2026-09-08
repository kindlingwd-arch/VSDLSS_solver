/* driver: RMAyQBnNFXSPAIPP addr=009a7520 size=162 */
#include "vsdlss_ref.h"


void RMAyQBnNFXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long *plVar13;
  long lVar14;
  long lVar15;
  
  lVar3 = *(long *)(param_3 + 0x28);
  plVar13 = (long *)(lVar3 + param_1 * 8);
  if (param_2 < *plVar13) {
    lVar4 = *(long *)(param_3 + 0x18);
    lVar12 = *(long *)(lVar4 + param_1 * 8);
    *plVar13 = param_2;
    if (1 < lVar12) {
      lVar14 = *(long *)(param_3 + 0x20);
      do {
        plVar13 = (long *)(lVar14 + lVar12 * 8);
        lVar9 = lVar12 >> 1;
        plVar1 = (long *)(lVar14 + lVar9 * 8);
        lVar11 = *plVar13;
        lVar5 = *plVar1;
        lVar10 = *(long *)(lVar3 + lVar5 * 8);
        lVar15 = *(long *)(lVar3 + lVar11 * 8);
        if (((lVar10 <= lVar15) && ((lVar10 < lVar15 || (lVar5 < lVar11)))) && (lVar9 != lVar12)) {
          return;
        }
        *plVar13 = lVar5;
        *plVar1 = lVar11;
        *(long *)(lVar4 + *plVar13 * 8) = lVar12;
        *(long *)(lVar4 + *plVar1 * 8) = lVar9;
        lVar12 = lVar9;
      } while (1 < lVar9);
    }
  }
  else if (*plVar13 < param_2) {
    lVar4 = *(long *)(param_3 + 0x18);
    lVar12 = *(long *)(lVar4 + param_1 * 8);
    *plVar13 = param_2;
    lVar14 = *(long *)(param_3 + 0x10);
    lVar11 = lVar12 * 2;
    if ((lVar11 <= lVar14) && (lVar11 != 0)) {
      lVar5 = *(long *)(param_3 + 0x20);
      while( true ) {
        lVar10 = lVar11;
        if (lVar11 < lVar14) {
          plVar1 = (long *)(lVar5 + lVar11 * 8);
          plVar2 = (long *)(lVar5 + 8 + lVar11 * 8);
          lVar9 = *plVar1;
          lVar6 = *plVar2;
          lVar7 = *(long *)(lVar3 + lVar9 * 8);
          lVar8 = *(long *)(lVar3 + lVar6 * 8);
          plVar13 = plVar1;
          lVar14 = lVar9;
          lVar15 = lVar7;
          if (((lVar8 <= lVar7) &&
              (lVar10 = lVar11 + 1, plVar13 = plVar2, lVar14 = lVar6, lVar15 = lVar8, lVar7 <= lVar8
              )) && (lVar10 = lVar11, plVar13 = plVar1, lVar14 = lVar9, lVar15 = lVar7,
                    lVar6 < lVar9)) {
            lVar10 = lVar11 + 1;
            plVar13 = plVar2;
            lVar14 = lVar6;
            lVar15 = lVar8;
          }
        }
        else {
          plVar13 = (long *)(lVar5 + lVar11 * 8);
          lVar14 = *plVar13;
          lVar15 = *(long *)(lVar3 + lVar14 * 8);
        }
        plVar1 = (long *)(lVar5 + lVar12 * 8);
        lVar11 = *plVar1;
        lVar9 = *(long *)(lVar3 + lVar11 * 8);
        if (((lVar9 <= lVar15) && ((lVar9 < lVar15 || (lVar11 < lVar14)))) &&
           (plVar13 = plVar1, lVar14 = lVar11, lVar10 != lVar12)) break;
        *plVar1 = lVar14;
        *plVar13 = lVar11;
        *(long *)(lVar4 + *plVar1 * 8) = lVar12;
        *(long *)(lVar4 + *plVar13 * 8) = lVar10;
        lVar14 = *(long *)(param_3 + 0x10);
        lVar11 = lVar10 * 2;
        if (lVar14 < lVar11) {
          return;
        }
        lVar12 = lVar10;
        if (lVar11 == 0) {
          return;
        }
      }
    }
  }
  return;
}


