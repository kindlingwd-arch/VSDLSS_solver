/* driver: yOOyVZyPBAZR_HBQZVbFDFQXSPAIPP addr=009b18d0 size=1be */
#include "vsdlss_ref.h"


undefined8 * yOOyVZyPBAZR_HBQZVbFDFQXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined8 *puVar5;
  long *plVar6;
  long lVar7;
  long *plVar8;
  long *plVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long local_68;
  
  puVar5 = (undefined8 *)KBTYOOZR_HBQYOOXSPAIPP(param_3);
  lVar1 = puVar5[3];
  lVar2 = puVar5[4];
  lVar10 = puVar5[2];
  uVar3 = *puVar5;
  plVar6 = (long *)(param_4 + 8);
  plVar8 = plVar6 + param_2;
  if (0 < param_2) {
    do {
      lVar7 = *plVar6;
      plVar6 = plVar6 + 1;
      plVar9 = (long *)(lVar1 + *(long *)(param_1 + lVar7 * 8) * 8);
      *plVar9 = *plVar9 + 1;
    } while (plVar6 != plVar8);
  }
  if (param_3 < 0x400) {
    local_68 = 1;
    if (param_3 < 1) goto LAB_009b19dd;
  }
  else {
    local_68 = param_3 >> 10;
  }
  lVar12 = 0;
  lVar7 = 0;
  lVar11 = 1;
  do {
    uVar4 = *(ulong *)(lVar1 + lVar11 * 8);
    if (lVar12 <= (long)uVar4) {
      lVar12 = local_68;
      if (local_68 <= (long)uVar4) {
        lVar12 = uVar4 + 1;
      }
      lVar7 = KBTZIL_HXSPAIPP(uVar3,lVar12 * 8);
    }
    *(long *)(lVar10 + lVar11 * 8) = lVar7;
    lVar11 = lVar11 + 1;
    lVar7 = lVar7 + (uVar4 + 1) * 8;
    lVar12 = lVar12 + ~uVar4;
  } while (lVar11 <= param_3);
LAB_009b19dd:
  plVar8 = (long *)(param_4 + 8);
  if (0 < param_2) {
    do {
      plVar9 = plVar8 + 1;
      lVar7 = *(long *)(param_1 + *plVar8 * 8);
      plVar6 = (long *)(lVar2 + lVar7 * 8);
      lVar12 = *(long *)(lVar10 + lVar7 * 8);
      lVar7 = *plVar6 + 1;
      *plVar6 = lVar7;
      *(long *)(lVar12 + lVar7 * 8) = *plVar8;
      plVar8 = plVar9;
    } while (plVar9 != (long *)(param_4 + 8) + param_2);
  }
  if (0 < param_3) {
    if (*(long *)(lVar2 + 8) == *(long *)(lVar1 + 8)) {
      lVar10 = 1;
      do {
        lVar10 = lVar10 + 1;
        if (param_3 < lVar10) {
          return puVar5;
        }
      } while (*(long *)(lVar2 + lVar10 * 8) == *(long *)(lVar1 + lVar10 * 8));
    }
    KOBOOLOXSPAIPP(3,"arrbuckarrutils.c",0x7f);
  }
  return puVar5;
}


