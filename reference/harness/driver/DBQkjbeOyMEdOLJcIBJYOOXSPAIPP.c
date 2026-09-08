/* driver: DBQkjbeOyMEdOLJcIBJYOOXSPAIPP addr=009a0360 size=325 */
#include "vsdlss_ref.h"


void DBQkjbeOyMEdOLJcIBJYOOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,undefined8 *param_5)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long *plVar11;
  long lVar12;
  long lVar13;
  ulong uVar14;
  long lVar15;
  long local_58 [2];
  long local_48;
  
  uVar4 = *(undefined8 *)(param_3 + 0x10);
  uVar5 = *(undefined8 *)(param_3 + 0x18);
  *param_5 = 0xffffffffffffffff;
  param_5[3] = 0;
  param_5[10] = 0;
  param_5[0xb] = 0;
  param_5[0xc] = 0;
  param_5[0xd] = 0;
  param_5[0xe] = 0;
  param_5[0xf] = 0;
  param_5[0x10] = 0;
  param_5[0x11] = 0;
  param_5[1] = 1;
  param_5[2] = param_1;
  uVar6 = KBTgtB_QLOXSPAIPP(param_2 * 2,0,0);
  param_5[0xd] = uVar6;
  lVar7 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  param_5[10] = lVar7;
  lVar8 = KBTgntB_QLOXSPAIPP(param_1,1,0);
  param_5[0xc] = lVar8;
  if (param_4 == 0) {
    lVar9 = KBTgtB_QLOXSPAIPP(param_2,0,0);
    OByAgtB_QLOdlRJXSPAIPP(uVar4,param_2,lVar9);
    lVar15 = KBTgtB_QLOXSPAIPP(param_2,0,0);
    OByAgtB_QLOdlRJXSPAIPP(uVar5,param_2,lVar15);
  }
  else {
    lVar9 = *(long *)(param_4 + 0x28);
    lVar15 = *(long *)(param_4 + 0x38);
  }
  _EB_HdOLJYKArLlLABPXSPAIPP(param_1,param_2,lVar9,lVar15);
  lVar12 = 1;
  if (0 < param_2) {
    do {
      plVar2 = (long *)(lVar7 + *(long *)(lVar9 + lVar12 * 8) * 8);
      *plVar2 = *plVar2 + 1;
      lVar1 = lVar12 * 8;
      lVar12 = lVar12 + 1;
      plVar2 = (long *)(lVar7 + *(long *)(lVar15 + lVar1) * 8);
      *plVar2 = *plVar2 + 1;
    } while (lVar12 != param_2 + 1);
  }
  if ((0 < param_1) && (*(undefined8 *)(lVar8 + 8) = param_5[0xd], param_1 != 1)) {
    lVar12 = 2;
    do {
      *(long *)(lVar8 + lVar12 * 8) =
           *(long *)(lVar8 + -8 + lVar12 * 8) + *(long *)(lVar7 + -8 + lVar12 * 8) * 8;
      lVar12 = lVar12 + 1;
    } while (lVar12 != param_1 + 1);
  }
  FKFQgtB_QLOXSPAIPP(lVar7,param_1,0);
  lVar12 = 1;
  if (0 < param_2) {
    do {
      lVar1 = *(long *)(lVar9 + lVar12 * 8);
      lVar13 = *(long *)(lVar15 + lVar12 * 8);
      lVar12 = lVar12 + 1;
      plVar2 = (long *)(lVar7 + lVar1 * 8);
      lVar10 = *plVar2 + 1;
      *plVar2 = lVar10;
      *(long *)(*(long *)(lVar8 + lVar1 * 8) + lVar10 * 8) = lVar13;
      plVar2 = (long *)(lVar7 + lVar13 * 8);
      lVar13 = *(long *)(lVar8 + lVar13 * 8);
      lVar10 = *plVar2 + 1;
      *plVar2 = lVar10;
      *(long *)(lVar13 + lVar10 * 8) = lVar1;
    } while (lVar12 != param_2 + 1);
  }
  if (param_4 == 0) {
    if (lVar9 != 0) {
      _yPFdOBBXSPAIPP(lVar9);
    }
    if (lVar15 != 0) {
      _yPFdOBBXSPAIPP(lVar15);
    }
  }
  lVar9 = param_5[0xd];
  KBTaLRKQFKDkyOHlLfByABO(local_58);
  if (param_1 < 1) {
    lVar7 = 0;
  }
  else {
    lVar15 = 1;
    uVar14 = 0;
    do {
      lVar1 = *(long *)(lVar7 + lVar15 * 8);
      lVar12 = lVar9 + uVar14 * 8;
      if (lVar1 < 1) {
        lVar13 = 0;
      }
      else {
        plVar11 = (long *)(*(long *)(lVar8 + lVar15 * 8) + 8);
        lVar13 = 0;
        plVar2 = plVar11 + lVar1;
        do {
          lVar1 = *plVar11;
          plVar3 = (long *)(local_48 + lVar1 * 8);
          if (*plVar3 < local_58[0]) {
            lVar13 = lVar13 + 1;
            *plVar3 = local_58[0];
            *(long *)(lVar12 + lVar13 * 8) = lVar1;
          }
          plVar11 = plVar11 + 1;
        } while (plVar11 != plVar2);
      }
      *(long *)(lVar7 + lVar15 * 8) = lVar13;
      *(long *)(lVar8 + lVar15 * 8) = lVar12;
      uVar14 = uVar14 + lVar13;
      lVar15 = lVar15 + 1;
      RKJyOHaLRKQFKDkyOHXSPAIPP(local_58);
    } while (lVar15 <= param_1);
    if ((uVar14 & 1) != 0) {
      KOBOOLOXSPAIPP(1,"mldorder.c",0x164a);
    }
    lVar7 = (long)uVar14 >> 1;
  }
  param_5[3] = lVar7;
  ABIBQBaLRKQFKDkyOHlLfByABO(local_58);
  return;
}


