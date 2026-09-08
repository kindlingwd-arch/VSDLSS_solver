/* driver: ANnNFXSPAIPP addr=009a76e0 size=16a */
#include "vsdlss_ref.h"


long ANnNFXSPAIPP(long param_1,undefined8 *param_2)

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
  long lVar13;
  long *plVar14;
  long lVar15;
  long lVar16;
  
  if (*(long *)(param_1 + 0x10) < 1) {
    KOBOOLOXSPAIPP(5,"priorityqutils_i.c",0x212);
  }
  lVar3 = *(long *)(*(long *)(param_1 + 0x20) + 8);
  *param_2 = *(undefined8 *)(*(long *)(param_1 + 0x28) + lVar3 * 8);
  MNFqTyMXSPAIPP(1,*(undefined8 *)(param_1 + 0x10),param_1);
  lVar15 = *(long *)(param_1 + 0x10) + -1;
  *(long *)(param_1 + 0x10) = lVar15;
  if (lVar15 < 2) {
    lVar12 = *(long *)(param_1 + 0x18);
  }
  else {
    lVar4 = *(long *)(param_1 + 0x28);
    lVar5 = *(long *)(param_1 + 0x20);
    lVar12 = *(long *)(param_1 + 0x18);
    lVar10 = 2;
    lVar16 = 1;
    do {
      lVar11 = lVar10;
      if (lVar10 < lVar15) {
        plVar2 = (long *)(lVar5 + lVar10 * 8);
        plVar1 = (long *)(lVar5 + 8 + lVar10 * 8);
        lVar6 = *plVar2;
        lVar7 = *plVar1;
        lVar8 = *(long *)(lVar4 + lVar6 * 8);
        lVar9 = *(long *)(lVar4 + lVar7 * 8);
        lVar13 = lVar8;
        plVar14 = plVar2;
        lVar15 = lVar6;
        if (((lVar9 <= lVar8) &&
            (lVar11 = lVar10 + 1, lVar13 = lVar9, plVar14 = plVar1, lVar15 = lVar7, lVar8 <= lVar9))
           && (lVar11 = lVar10, lVar13 = lVar8, plVar14 = plVar2, lVar15 = lVar6, lVar7 < lVar6)) {
          lVar11 = lVar10 + 1;
          lVar13 = lVar9;
          plVar14 = plVar1;
          lVar15 = lVar7;
        }
      }
      else {
        plVar14 = (long *)(lVar5 + lVar10 * 8);
        lVar15 = *plVar14;
        lVar13 = *(long *)(lVar4 + lVar15 * 8);
      }
      plVar1 = (long *)(lVar5 + lVar16 * 8);
      lVar10 = *plVar1;
      lVar6 = *(long *)(lVar4 + lVar10 * 8);
      if (((lVar6 <= lVar13) && ((lVar6 < lVar13 || (lVar10 < lVar15)))) &&
         (plVar14 = plVar1, lVar15 = lVar10, lVar11 != lVar16)) break;
      *plVar1 = lVar15;
      *plVar14 = lVar10;
      lVar10 = lVar11 * 2;
      *(long *)(lVar12 + *plVar1 * 8) = lVar16;
      *(long *)(lVar12 + *plVar14 * 8) = lVar11;
      lVar15 = *(long *)(param_1 + 0x10);
      lVar16 = lVar11;
    } while (lVar10 <= lVar15);
  }
  *(undefined8 *)(lVar12 + lVar3 * 8) = 0xffffffffffffffff;
  return lVar3;
}


