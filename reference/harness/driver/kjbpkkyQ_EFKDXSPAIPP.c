/* driver: kjbpkkyQ_EFKDXSPAIPP addr=00997ca0 size=14d */
#include "vsdlss_ref.h"


long kjbpkkyQ_EFKDXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long *plVar12;
  long lVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  
  lVar4 = *(long *)(param_2 + 0x50);
  lVar9 = *(long *)(param_2 + 0x10);
  lVar5 = *(long *)(param_2 + 0x88);
  lVar6 = *(long *)(param_2 + 0x80);
  FKFQgtB_QLOXSPAIPP(param_3,lVar9,0xffffffffffffffff);
  if (lVar9 < 1) {
    lVar9 = 0;
  }
  else {
    plVar2 = (long *)(param_4 + 8 + lVar9 * 8);
    plVar15 = (long *)(param_4 + 8);
    lVar9 = 0;
    do {
      while( true ) {
        lVar7 = *plVar15;
        lVar1 = lVar7 * 8;
        plVar3 = (long *)(param_3 + lVar1);
        if (*plVar3 != -1) break;
        if (lVar6 == 0) {
          lVar16 = 1;
        }
        else {
          lVar16 = *(long *)(lVar6 + lVar7 * 8);
        }
        lVar8 = *(long *)(lVar4 + lVar7 * 8);
        lVar10 = lVar7;
        plVar12 = plVar3;
        lVar13 = lVar1;
        if (0 < lVar8) {
          lVar11 = 1;
          do {
            lVar10 = *(long *)(*(long *)(*(long *)(param_2 + 0x60) + lVar7 * 8) + lVar11 * 8);
            lVar14 = 1;
            if (lVar6 != 0) {
              lVar14 = *(long *)(lVar6 + lVar10 * 8);
            }
            plVar12 = (long *)(param_3 + lVar10 * 8);
          } while (((*plVar12 != -1) || (lVar13 = lVar10 * 8, param_1 < lVar14 + lVar16)) &&
                  (lVar11 = lVar11 + 1, lVar10 = lVar7, plVar12 = plVar3, lVar13 = lVar1,
                  lVar11 <= lVar8));
        }
        lVar9 = lVar9 + 1;
        plVar15 = plVar15 + 1;
        *(long *)(lVar5 + lVar7 * 8) = lVar9;
        *(long *)(lVar5 + lVar13) = lVar9;
        *plVar12 = lVar7;
        *plVar3 = lVar10;
        if (plVar15 == plVar2) {
          return lVar9;
        }
      }
      plVar15 = plVar15 + 1;
    } while (plVar15 != plVar2);
  }
  return lVar9;
}


