/* driver: zR_HBQlBFDEzLOPgKQLlLABZR_HBQPXSPAIPP addr=009b1ab0 size=1f9 */
#include "vsdlss_ref.h"


undefined8 * zR_HBQlBFDEzLOPgKQLlLABZR_HBQPXSPAIPP(long *param_1)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  ulong uVar9;
  undefined8 *puVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long local_68;
  
  lVar2 = param_1[1];
  lVar12 = *param_1;
  lVar3 = param_1[5];
  lVar4 = param_1[7];
  puVar10 = (undefined8 *)KBTYOOZR_HBQYOOXSPAIPP(lVar2);
  lVar5 = puVar10[3];
  lVar6 = puVar10[4];
  lVar7 = puVar10[2];
  uVar8 = *puVar10;
  lVar11 = 1;
  if (0 < lVar12) {
    do {
      lVar13 = lVar11 * 8;
      lVar14 = *(long *)(lVar4 + lVar11 * 8);
      lVar11 = lVar11 + 1;
      plVar1 = (long *)(lVar5 + *(long *)(lVar3 + lVar13) * 8);
      *plVar1 = *plVar1 + 1;
      plVar1 = (long *)(lVar5 + lVar14 * 8);
      *plVar1 = *plVar1 + 1;
    } while (lVar11 != lVar12 + 1);
  }
  if (lVar2 < 0x400) {
    local_68 = 1;
    if (lVar2 < 1) goto LAB_009b1be3;
  }
  else {
    local_68 = lVar2 >> 10;
  }
  lVar13 = 0;
  lVar11 = 0;
  lVar14 = 1;
  do {
    uVar9 = *(ulong *)(lVar5 + lVar14 * 8);
    if (lVar13 <= (long)uVar9) {
      lVar13 = local_68;
      if (local_68 <= (long)uVar9) {
        lVar13 = uVar9 + 1;
      }
      lVar11 = KBTZIL_HXSPAIPP(uVar8,lVar13 * 8);
    }
    *(long *)(lVar7 + lVar14 * 8) = lVar11;
    lVar14 = lVar14 + 1;
    lVar11 = lVar11 + (uVar9 + 1) * 8;
    lVar13 = lVar13 + ~uVar9;
  } while (lVar14 <= lVar2);
LAB_009b1be3:
  lVar11 = 1;
  if (0 < lVar12) {
    do {
      lVar13 = *(long *)(lVar3 + lVar11 * 8);
      lVar14 = *(long *)(lVar4 + lVar11 * 8);
      lVar11 = lVar11 + 1;
      plVar1 = (long *)(lVar6 + lVar13 * 8);
      lVar15 = *plVar1 + 1;
      *plVar1 = lVar15;
      *(long *)(*(long *)(lVar7 + lVar13 * 8) + lVar15 * 8) = lVar14;
      plVar1 = (long *)(lVar6 + lVar14 * 8);
      lVar14 = *(long *)(lVar7 + lVar14 * 8);
      lVar15 = *plVar1 + 1;
      *plVar1 = lVar15;
      *(long *)(lVar14 + lVar15 * 8) = lVar13;
    } while (lVar11 != lVar12 + 1);
  }
  if (0 < lVar2) {
    if (*(long *)(lVar6 + 8) == *(long *)(lVar5 + 8)) {
      lVar12 = 1;
      do {
        lVar12 = lVar12 + 1;
        if (lVar2 < lVar12) {
          return puVar10;
        }
      } while (*(long *)(lVar6 + lVar12 * 8) == *(long *)(lVar5 + lVar12 * 8));
    }
    KOBOOLOXSPAIPP(3,"arrbuckarrutils.c",0xce);
  }
  return puVar10;
}


