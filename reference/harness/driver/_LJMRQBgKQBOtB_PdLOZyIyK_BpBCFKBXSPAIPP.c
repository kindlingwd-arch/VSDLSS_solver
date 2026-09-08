/* driver: _LJMRQBgKQBOtB_PdLOZyIyK_BpBCFKBXSPAIPP addr=009991a0 size=33b */
#include "vsdlss_ref.h"


void _LJMRQBgKQBOtB_PdLOZyIyK_BpBCFKBXSPAIPP(undefined8 param_1,long param_2,long param_3)

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
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long *plVar20;
  long lVar21;
  ulong local_50;
  
  lVar18 = *(long *)(param_2 + 0x50);
  lVar3 = *(long *)(param_2 + 0x10);
  lVar4 = *(long *)(param_2 + 0x80);
  lVar17 = *(long *)(param_3 + 0xb0);
  lVar14 = *(long *)(param_3 + 0x90);
  lVar5 = *(long *)(param_3 + 0xa8);
  plVar20 = *(long **)(param_3 + 0x98);
  lVar6 = *(long *)(param_3 + 0xa0);
  lVar7 = *(long *)(param_2 + 0x60);
  lVar21 = *(long *)(param_2 + 8);
  lVar16 = *(long *)(param_2 + 0x70);
  FKFQgtB_QLOXSPAIPP(lVar14,lVar3,0);
  FKFQgtB_QLOXSPAIPP(plVar20,lVar3,0);
  FKFQgtB_QLOXSPAIPP(lVar5,lVar3,0);
  *(undefined8 *)(param_2 + 0x38) = 0;
  *(undefined8 *)(param_2 + 0x40) = 0;
  if ((lVar16 == 0) && (lVar21 == 1)) {
    if (lVar3 < 1) {
LAB_009994cb:
      *(undefined8 *)(param_2 + 0x28) = 0;
      *(undefined8 *)(param_2 + 0x20) = 0;
      return;
    }
    lVar16 = 1;
    local_50 = 0;
    lVar21 = 0;
    do {
      plVar20 = plVar20 + 1;
      lVar19 = *(long *)(lVar6 + lVar16 * 8);
      lVar8 = *(long *)(lVar7 + lVar16 * 8);
      plVar1 = (long *)(param_2 + 0x30 + lVar19 * 8);
      *plVar1 = *plVar1 + 1;
      plVar13 = (long *)(lVar8 + 8);
      lVar8 = *(long *)(lVar18 + lVar16 * 8);
      plVar1 = plVar13 + lVar8;
      if (0 < lVar8) {
        do {
          while (*(long *)(lVar6 + *plVar13 * 8) == lVar19) {
            plVar13 = plVar13 + 1;
            plVar2 = (long *)(lVar14 + lVar16 * 8);
            *plVar2 = *plVar2 + 1;
            if (plVar13 == plVar1) goto LAB_009992f5;
          }
          plVar13 = plVar13 + 1;
          *plVar20 = *plVar20 + 1;
        } while (plVar13 != plVar1);
      }
LAB_009992f5:
      if (0 < *plVar20) {
        local_50 = local_50 + *plVar20;
        lVar21 = lVar21 + 1;
        *(long *)(lVar17 + lVar21 * 8) = lVar16;
        *(long *)(lVar5 + lVar16 * 8) = lVar21;
      }
      lVar16 = lVar16 + 1;
    } while (lVar16 != lVar3 + 1);
  }
  else {
    if (lVar3 < 1) goto LAB_009994cb;
    lVar19 = 1;
    local_50 = 0;
    lVar21 = 0;
    do {
      plVar20 = plVar20 + 1;
      lVar9 = *(long *)(lVar6 + lVar19 * 8);
      plVar1 = (long *)(param_2 + 0x30 + lVar9 * 8);
      *plVar1 = *plVar1 + *(long *)(lVar4 + lVar19 * 8);
      lVar10 = *(long *)(lVar18 + lVar19 * 8);
      lVar11 = *(long *)(lVar7 + lVar19 * 8);
      lVar8 = lVar10 + 1;
      lVar12 = *(long *)(lVar16 + lVar19 * 8);
      lVar15 = 1;
      if (0 < lVar10) {
        do {
          while (lVar10 = *(long *)(lVar12 + lVar15 * 8),
                *(long *)(lVar6 + *(long *)(lVar11 + lVar15 * 8) * 8) == lVar9) {
            lVar15 = lVar15 + 1;
            plVar1 = (long *)(lVar14 + lVar19 * 8);
            *plVar1 = *plVar1 + lVar10;
            if (lVar15 == lVar8) goto LAB_00999450;
          }
          lVar15 = lVar15 + 1;
          *plVar20 = *plVar20 + lVar10;
        } while (lVar15 != lVar8);
      }
LAB_00999450:
      if (0 < *plVar20) {
        local_50 = local_50 + *plVar20;
        lVar21 = lVar21 + 1;
        *(long *)(lVar17 + lVar21 * 8) = lVar19;
        *(long *)(lVar5 + lVar19 * 8) = lVar21;
      }
      lVar19 = lVar19 + 1;
    } while (lVar19 != lVar3 + 1);
  }
  if ((local_50 & 1) == 0) {
    lVar18 = *(long *)(param_2 + 0x38) + *(long *)(param_2 + 0x40);
  }
  else {
    KOBOOLOXSPAIPP(1,"mldorder.c",0x87f);
    lVar18 = *(long *)(param_2 + 0x38) + *(long *)(param_2 + 0x40);
  }
  lVar17 = 0;
  *(long *)(param_2 + 0x28) = (long)local_50 >> 1;
  *(long *)(param_2 + 0x20) = lVar21;
  if (0 < lVar3) {
    lVar14 = 1;
    do {
      while (lVar4 == 0) {
        lVar14 = lVar14 + 1;
        lVar17 = lVar17 + 1;
        if (lVar14 == lVar3 + 1) goto LAB_00999390;
      }
      lVar5 = lVar14 * 8;
      lVar14 = lVar14 + 1;
      lVar17 = lVar17 + *(long *)(lVar4 + lVar5);
    } while (lVar14 != lVar3 + 1);
  }
LAB_00999390:
  if (lVar17 == lVar18) {
    return;
  }
  KOBOOLOXSPAIPP(1,"mldorder.c",0x88c);
  return;
}


