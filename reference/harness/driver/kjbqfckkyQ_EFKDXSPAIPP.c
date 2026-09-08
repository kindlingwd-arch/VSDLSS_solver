/* driver: kjbqfckkyQ_EFKDXSPAIPP addr=00997f60 size=1e6 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long kjbqfckkyQ_EFKDXSPAIPP
               (long param_1,long param_2,long param_3,undefined8 param_4,long param_5,
               undefined8 param_6)

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
  long *plVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  
  lVar3 = *(long *)(param_2 + 0x50);
  lVar4 = *(long *)(param_2 + 0x60);
  lVar11 = *(long *)(param_2 + 0x10);
  lVar15 = *(long *)(param_2 + 0x18);
  lVar5 = *(long *)(param_2 + 0x80);
  lVar6 = *(long *)(param_2 + 0x70);
  if ((lVar6 == 0) || (lVar5 == 0)) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0x57c);
  }
  lVar10 = (long)((double)((lVar15 * 2) / lVar11) * _DAT_00e8e620);
  lVar15 = 1;
  if (0 < lVar10) {
    lVar15 = lVar10;
  }
  zR_HBQlLABPZVbBDOBBXSPAIPP(lVar15 + 1,lVar11,param_4,lVar3,param_5,param_6);
  lVar15 = *(long *)(param_2 + 0x88);
  FKFQgtB_QLOXSPAIPP(param_3,lVar11,0xffffffffffffffff);
  if (lVar11 < 1) {
    lVar11 = 0;
  }
  else {
    plVar2 = (long *)(param_5 + 8 + lVar11 * 8);
    plVar13 = (long *)(param_5 + 8);
    lVar11 = 0;
    do {
      while( true ) {
        lVar7 = *plVar13;
        lVar10 = lVar7 * 8;
        plVar1 = (long *)(param_3 + lVar10);
        if (*plVar1 != -1) break;
        lVar16 = *(long *)(lVar3 + lVar7 * 8);
        plVar14 = plVar1;
        lVar17 = lVar7;
        if (0 < lVar16) {
          lVar16 = lVar16 + 1;
          lVar10 = 0;
          lVar12 = 1;
          do {
            while( true ) {
              lVar8 = *(long *)(*(long *)(lVar4 + lVar7 * 8) + lVar12 * 8);
              lVar9 = *(long *)(*(long *)(lVar6 + lVar7 * 8) + lVar12 * 8);
              if ((*(long *)(param_3 + lVar8 * 8) != -1) || (lVar9 <= lVar10)) break;
              if (*(long *)(lVar5 + lVar8 * 8) <= param_1 - *(long *)(lVar5 + lVar7 * 8)) {
                lVar17 = lVar8;
                lVar10 = lVar9;
              }
              lVar12 = lVar12 + 1;
              if (lVar12 == lVar16) goto LAB_009980d8;
            }
            lVar12 = lVar12 + 1;
          } while (lVar12 != lVar16);
LAB_009980d8:
          lVar10 = lVar17 * 8;
          plVar14 = (long *)(param_3 + lVar10);
        }
        lVar11 = lVar11 + 1;
        plVar13 = plVar13 + 1;
        *(long *)(lVar15 + lVar7 * 8) = lVar11;
        *(long *)(lVar15 + lVar10) = lVar11;
        *plVar14 = lVar7;
        *plVar1 = lVar17;
        if (plVar13 == plVar2) {
          return lVar11;
        }
      }
      plVar13 = plVar13 + 1;
    } while (plVar13 != plVar2);
  }
  return lVar11;
}


