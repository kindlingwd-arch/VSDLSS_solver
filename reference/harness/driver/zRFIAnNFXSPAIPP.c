/* driver: zRFIAnNFXSPAIPP addr=009a7960 size=150 */
#include "vsdlss_ref.h"


void zRFIAnNFXSPAIPP(long param_1,long param_2)

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
  long *plVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  
  *(long *)(param_1 + 0x10) = param_2;
  if (0 < param_2) {
    lVar12 = *(long *)(param_1 + 0x18);
    lVar16 = *(long *)(param_1 + 0x20);
    lVar8 = 1;
    do {
      *(long *)(lVar12 + *(long *)(lVar16 + lVar8 * 8) * 8) = lVar8;
      lVar8 = lVar8 + 1;
    } while (lVar8 != param_2 + 1);
    param_2 = *(long *)(param_1 + 0x10);
  }
  lVar12 = param_2 / 2;
  if (lVar12 < 1) {
    return;
  }
  lVar16 = lVar12 * 2;
  do {
    if (lVar16 <= param_2) {
      lVar8 = *(long *)(param_1 + 0x20);
      lVar3 = *(long *)(param_1 + 0x28);
      lVar10 = lVar16;
      lVar14 = lVar12;
      do {
        lVar9 = lVar10;
        if (lVar10 < param_2) {
          plVar1 = (long *)(lVar8 + lVar10 * 8);
          plVar2 = (long *)(lVar8 + 8 + lVar10 * 8);
          lVar4 = *plVar1;
          lVar5 = *plVar2;
          lVar6 = *(long *)(lVar3 + lVar4 * 8);
          lVar7 = *(long *)(lVar3 + lVar5 * 8);
          plVar11 = plVar1;
          lVar13 = lVar4;
          lVar15 = lVar6;
          if (((lVar7 <= lVar6) &&
              (lVar9 = lVar10 + 1, plVar11 = plVar2, lVar13 = lVar5, lVar15 = lVar7, lVar6 <= lVar7)
              ) && (lVar9 = lVar10, plVar11 = plVar1, lVar13 = lVar4, lVar15 = lVar6, lVar5 < lVar4)
             ) {
            lVar9 = lVar10 + 1;
            plVar11 = plVar2;
            lVar13 = lVar5;
            lVar15 = lVar7;
          }
        }
        else {
          plVar11 = (long *)(lVar8 + lVar10 * 8);
          lVar13 = *plVar11;
          lVar15 = *(long *)(lVar3 + lVar13 * 8);
        }
        plVar1 = (long *)(lVar8 + lVar14 * 8);
        lVar10 = *plVar1;
        lVar4 = *(long *)(lVar3 + lVar10 * 8);
        if (((lVar4 <= lVar15) && ((lVar4 < lVar15 || (lVar10 < lVar13)))) &&
           (plVar11 = plVar1, lVar13 = lVar10, lVar9 != lVar14)) break;
        *plVar1 = lVar13;
        *plVar11 = lVar10;
        lVar13 = *(long *)(param_1 + 0x18);
        *(long *)(lVar13 + *plVar1 * 8) = lVar14;
        lVar10 = lVar9 * 2;
        *(long *)(lVar13 + *plVar11 * 8) = lVar9;
        param_2 = *(long *)(param_1 + 0x10);
        lVar14 = lVar9;
      } while (lVar10 <= param_2);
    }
    lVar16 = lVar16 + -2;
    lVar12 = lVar12 + -1;
    if (lVar12 == 0) {
      return;
    }
    param_2 = *(long *)(param_1 + 0x10);
  } while( true );
}


