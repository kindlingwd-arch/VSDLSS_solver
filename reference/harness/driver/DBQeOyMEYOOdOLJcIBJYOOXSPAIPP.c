/* driver: DBQeOyMEYOOdOLJcIBJYOOXSPAIPP addr=00968430 size=2b1 */
#include "vsdlss_ref.h"


void DBQeOyMEYOOdOLJcIBJYOOXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long *plVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  long lVar9;
  long lVar10;
  long *plVar11;
  long lVar12;
  long lVar13;
  ulong uVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  
  lVar7 = KBTeOyMEXSPAIPP();
  uVar3 = *(undefined8 *)(param_3 + 0x10);
  uVar4 = *(undefined8 *)(param_3 + 0x18);
  uVar8 = KBTgtB_QLOXSPAIPP(param_2 * 2,0,0);
  *(undefined8 *)(lVar7 + 0x28) = uVar8;
  lVar9 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(long *)(lVar7 + 0x10) = lVar9;
  lVar10 = KBTgntB_QLOXSPAIPP(param_1,1,0);
  *(long *)(lVar7 + 0x30) = lVar10;
  if (param_4 == 0) {
    lVar12 = KBTgtB_QLOXSPAIPP(param_2,0,0);
    OByAgtB_QLOdlRJXSPAIPP(uVar3,param_2,lVar12);
    lVar17 = KBTgtB_QLOXSPAIPP(param_2,0,0);
    OByAgtB_QLOdlRJXSPAIPP(uVar4,param_2,lVar17);
  }
  else {
    lVar12 = *(long *)(param_4 + 0x28);
    lVar17 = *(long *)(param_4 + 0x38);
  }
  _EB_HdOLJYKArLlLABPXSPAIPP(param_1,param_2,lVar12,lVar17);
  lVar15 = 1;
  if (0 < param_2) {
    do {
      plVar11 = (long *)(lVar9 + *(long *)(lVar12 + lVar15 * 8) * 8);
      *plVar11 = *plVar11 + 1;
      lVar1 = lVar15 * 8;
      lVar15 = lVar15 + 1;
      plVar11 = (long *)(lVar9 + *(long *)(lVar17 + lVar1) * 8);
      *plVar11 = *plVar11 + 1;
    } while (lVar15 != param_2 + 1);
  }
  if ((0 < param_1) && (*(undefined8 *)(lVar10 + 8) = *(undefined8 *)(lVar7 + 0x28), param_1 != 1))
  {
    lVar15 = 2;
    do {
      *(long *)(lVar10 + lVar15 * 8) =
           *(long *)(lVar10 + -8 + lVar15 * 8) + *(long *)(lVar9 + -8 + lVar15 * 8) * 8;
      lVar15 = lVar15 + 1;
    } while (lVar15 != param_1 + 1);
  }
  FKFQgtB_QLOXSPAIPP(lVar9,param_1,0);
  lVar15 = 1;
  if (0 < param_2) {
    do {
      lVar5 = *(long *)(lVar12 + lVar15 * 8);
      lVar16 = *(long *)(lVar17 + lVar15 * 8);
      lVar15 = lVar15 + 1;
      plVar11 = (long *)(lVar9 + lVar5 * 8);
      lVar1 = *plVar11 + 1;
      *plVar11 = lVar1;
      *(long *)(*(long *)(lVar10 + lVar5 * 8) + lVar1 * 8) = lVar16;
      plVar11 = (long *)(lVar9 + lVar16 * 8);
      lVar16 = *(long *)(lVar10 + lVar16 * 8);
      lVar1 = *plVar11 + 1;
      *plVar11 = lVar1;
      *(long *)(lVar16 + lVar1 * 8) = lVar5;
    } while (lVar15 != param_2 + 1);
  }
  if (param_4 == 0) {
    if (lVar12 != 0) {
      _yPFdOBBXSPAIPP(lVar12);
    }
    if (lVar17 != 0) {
      _yPFdOBBXSPAIPP(lVar17);
    }
  }
  plVar11 = (long *)KBTaLRKQFKDkyOHXSPAIPP(param_1);
  lVar12 = plVar11[2];
  if (param_1 < 1) {
    uVar14 = 0;
  }
  else {
    lVar17 = 1;
    uVar14 = 0;
    do {
      lVar15 = *(long *)(lVar9 + lVar17 * 8);
      lVar1 = *plVar11;
      lVar5 = *(long *)(lVar10 + lVar17 * 8);
      if (lVar15 < 1) {
        lVar16 = 0;
      }
      else {
        lVar13 = 1;
        lVar16 = 0;
        do {
          lVar6 = *(long *)(lVar5 + lVar13 * 8);
          plVar2 = (long *)(lVar12 + lVar6 * 8);
          if (*plVar2 < lVar1) {
            lVar16 = lVar16 + 1;
            *plVar2 = lVar1;
            *(long *)(lVar5 + lVar16 * 8) = lVar6;
          }
          lVar13 = lVar13 + 1;
        } while (lVar13 != lVar15 + 1);
      }
      *(long *)(lVar9 + lVar17 * 8) = lVar16;
      uVar14 = uVar14 + lVar16;
      lVar17 = lVar17 + 1;
      RKJyOHaLRKQFKDkyOHXSPAIPP(plVar11);
    } while (lVar17 <= param_1);
    if ((uVar14 & 1) != 0) {
      KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x74);
    }
  }
  ABIBQBaLRKQFKDkyOHXSPAIPP(plVar11);
  *(ulong *)(lVar7 + 8) = uVar14;
  return;
}


