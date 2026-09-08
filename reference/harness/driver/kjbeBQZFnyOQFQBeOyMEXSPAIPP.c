/* driver: kjbeBQZFnyOQFQBeOyMEXSPAIPP addr=0099b0f0 size=391 */
#include "vsdlss_ref.h"


long kjbeBQZFnyOQFQBeOyMEXSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  long lVar11;
  undefined8 uVar12;
  long lVar13;
  long *plVar14;
  long *plVar15;
  long lVar16;
  ulong uVar17;
  long lVar18;
  long lVar19;
  long *plVar20;
  long local_80;
  ulong local_78;
  
  uVar12 = *(undefined8 *)(param_2 + 0x10);
  lVar18 = *(long *)(param_3 + 0xb0);
  lVar2 = *(long *)(param_2 + 0x60);
  lVar3 = *(long *)(param_3 + 0xa0);
  lVar4 = *(long *)(param_3 + 0x80);
  lVar16 = *(long *)(param_2 + 0x20);
  lVar5 = *(long *)(param_2 + 0x50);
  *(undefined8 *)(param_4 + 8) = 0;
  *(undefined8 *)(param_4 + 0x10) = 0;
  if (lVar16 < 1) {
    local_78 = 0;
    local_80 = 0;
  }
  else {
    plVar10 = (long *)(lVar18 + 8);
    lVar19 = 0;
    plVar15 = plVar10 + lVar16;
    do {
      lVar11 = *plVar10;
      plVar10 = plVar10 + 1;
      plVar1 = (long *)(param_4 + *(long *)(lVar3 + lVar11 * 8) * 8);
      *plVar1 = *plVar1 + 1;
      lVar19 = lVar19 + *(long *)(lVar5 + lVar11 * 8);
    } while (plVar10 != plVar15);
    local_80 = *(long *)(param_4 + 8) + *(long *)(param_4 + 0x10);
    local_78 = lVar19 - (lVar19 >> 0x3f) & 0xfffffffffffffffe;
  }
  if (lVar16 != local_80) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0xc14);
  }
  lVar19 = QyDlBTkjbeOyMEfByABOmKIVXSPAIPP(param_1);
  lVar11 = QyDlBTgtB_QLOXSPAIPP(param_1,local_80,1,0);
  *(long *)(lVar19 + 0x10) = local_80;
  *(long *)(lVar19 + 0x50) = lVar11;
  uVar6 = *(undefined8 *)(param_4 + 8);
  *(undefined8 *)(param_4 + 8) = 0;
  *(undefined8 *)(param_4 + 0x10) = uVar6;
  FKFQgtB_QLOXSPAIPP(lVar4,uVar12,0);
  FKFQgtB_QLOXSPAIPP(param_5,lVar16,0);
  plVar15 = (long *)(lVar18 + 8);
  plVar10 = plVar15 + lVar16;
  if (0 < lVar16) {
    do {
      lVar13 = *plVar15;
      plVar15 = plVar15 + 1;
      plVar1 = (long *)(param_4 + *(long *)(lVar3 + lVar13 * 8) * 8);
      lVar7 = *plVar1;
      *plVar1 = lVar7 + 1;
      *(long *)(lVar4 + lVar13 * 8) = lVar7 + 1;
      *(long *)(param_5 + *plVar1 * 8) = lVar13;
      *(undefined8 *)(lVar11 + *plVar1 * 8) = *(undefined8 *)(lVar5 + lVar13 * 8);
    } while (plVar15 != plVar10);
  }
  if (*(long *)(param_4 + 0x10) != local_80) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0xc2e);
  }
  uVar12 = QyDlBTgntB_QLOXSPAIPP(param_1,local_80,1,0);
  *(undefined8 *)(lVar19 + 0x60) = uVar12;
  uVar12 = QyDlBTgtB_QLOXSPAIPP(param_1,local_78,0,0);
  *(undefined8 *)(lVar19 + 0x68) = uVar12;
  *(undefined8 *)(*(long *)(lVar19 + 0x60) + 8) = uVar12;
  lVar13 = 8;
  if (1 < local_80) {
    do {
      lVar7 = lVar13 + 8;
      *(long *)(*(long *)(lVar19 + 0x60) + 8 + lVar13) =
           *(long *)(*(long *)(lVar19 + 0x60) + lVar13) + *(long *)(lVar11 + lVar13) * 8;
      lVar13 = lVar7;
    } while (lVar7 != local_80 * 8);
  }
  FKFQgtB_QLOXSPAIPP(lVar11,local_80,0);
  if (lVar16 < 1) {
    lVar18 = 0;
  }
  else {
    lVar13 = *(long *)(lVar19 + 0x60);
    plVar10 = (long *)(lVar18 + 8);
    plVar15 = plVar10 + lVar16;
    uVar17 = 0;
    do {
      lVar18 = *plVar10;
      lVar16 = *(long *)(lVar5 + lVar18 * 8);
      lVar7 = *(long *)(lVar3 + lVar18 * 8);
      lVar8 = *(long *)(lVar4 + lVar18 * 8);
      lVar9 = *(long *)(lVar13 + lVar8 * 8);
      if (0 < lVar16) {
        plVar14 = (long *)(*(long *)(lVar2 + lVar18 * 8) + 8);
        plVar20 = (long *)(lVar8 * 8 + lVar11);
        plVar1 = plVar14 + lVar16;
        do {
          lVar18 = *plVar14;
          if (*(long *)(lVar3 + lVar18 * 8) != lVar7) {
            uVar17 = uVar17 + 1;
            lVar16 = *plVar20 + 1;
            *plVar20 = lVar16;
            *(undefined8 *)(lVar9 + lVar16 * 8) = *(undefined8 *)(lVar4 + lVar18 * 8);
          }
          plVar14 = plVar14 + 1;
        } while (plVar14 != plVar1);
      }
      plVar10 = plVar10 + 1;
    } while (plVar10 != plVar15);
    lVar18 = (long)uVar17 >> 1;
    if ((uVar17 & 1) != 0) {
      KOBOOLOXSPAIPP(1,"mldorder.c",0xc52);
    }
  }
  *(long *)(lVar19 + 0x18) = lVar18;
  return lVar19;
}


