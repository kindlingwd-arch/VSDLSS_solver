/* driver: kjbmOABOgKQBOKyIXSPAIPP addr=0099fc20 size=72c */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void kjbmOABOgKQBOKyIXSPAIPP
               (long param_1,long param_2,undefined8 param_3,long *param_4,undefined8 param_5,
               undefined8 param_6)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  long lVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  ulong uVar21;
  long lVar22;
  long lVar23;
  ulong uVar24;
  long *plVar25;
  long *plVar26;
  long lVar27;
  double dVar28;
  double dVar29;
  double dVar30;
  double dVar31;
  long local_68;
  long local_40 [2];
  
  lVar2 = *(long *)(param_1 + 0x10);
  lVar27 = *param_4;
  lVar11 = QyDlBTkjbmOABOtB_P(param_5,lVar2);
  uVar16 = *(undefined8 *)(lVar11 + 0x58);
  dVar31 = (double)lVar2 * _DAT_00b1ab28;
  FKFQgtB_QLOXSPAIPP(uVar16,lVar2,0);
  uVar17 = *(undefined8 *)(lVar11 + 0x50);
  FKFQgtB_QLOXSPAIPP(uVar17,lVar2,0);
  uVar3 = *(undefined8 *)(lVar11 + 0x70);
  FKFQgtB_QLOXSPAIPP(uVar3,lVar2,0);
  uVar12 = *(undefined8 *)(lVar11 + 0x90);
  FKFQgtB_QLOXSPAIPP(uVar12,lVar2,0);
  lVar18 = *(long *)(lVar11 + 0x40);
  *(undefined8 *)(lVar18 + 0x10) = *(undefined8 *)(lVar11 + 0x98);
  FKFQaLRKQFKDkyOHXSPAIPP(lVar2,lVar18);
  lVar19 = *(long *)(lVar11 + 0x30);
  uVar13 = *(undefined8 *)(lVar11 + 0x60);
  uVar14 = *(undefined8 *)(lVar11 + 0xa8);
  uVar15 = *(undefined8 *)(lVar11 + 0x68);
  *(long *)(lVar19 + 0x10) = lVar2;
  *(undefined8 *)(lVar19 + 0x18) = uVar13;
  FKFQoXSPAIPP(lVar19);
  uVar12 = CFKAaLKKaLJMPYKAkyHBaLKQFDX2
                     (param_6,lVar27,param_2 + (lVar2 - lVar27) * 8,param_1,param_3,uVar16,uVar17,
                      uVar3,uVar12,lVar2,uVar14,uVar15,*(undefined8 *)(lVar18 + 0x10),lVar19);
  uVar13 = QyDlBTYqQy_H(param_5,lVar2);
  uVar14 = QyDlBTYqQy_H(param_5,lVar2);
  uVar15 = QyDlBTYqQy_H(param_5,lVar2);
  kjbnRPEaLJMLKBKQPXSPAIPP(uVar12,uVar16,uVar17,uVar3,uVar13,uVar14,uVar15);
  PBQaLRKQFKDkyOHrLlRIIXSPAIPP(lVar18);
  PBQorLlRIIXSPAIPP(lVar19);
  uVar16 = QyDlBTkjbeOyMEfByABOmKIVXSPAIPP(param_5);
  dVar28 = _DAT_00ab67f8;
  local_68 = 1;
  if (((1 < lVar2) && (0.0 < DAT_00e8e608)) && (DAT_00e8e608 < _DAT_00ab67f8)) {
    dVar29 = log((double)lVar2);
    dVar30 = log(DAT_00e8e608);
    dVar28 = dVar28 - dVar29 / dVar30;
    local_68 = (long)(dVar28 + dVar28);
  }
  uVar17 = QyDlBTkjbeOyMEtB_QLO(param_5,local_68,1,0);
  lVar18 = lVar2 + 0x1f;
  if (-1 < lVar2) {
    lVar18 = lVar2;
  }
LAB_0099fec8:
  do {
    lVar19 = BJMQVYqQy_HXSPAIPP(uVar13);
    while( true ) {
      if (lVar19 != 0) {
        *param_4 = lVar27;
        return;
      }
      dVar28 = (double)DBQryDaROOqFWBZVQBPXSPAIPP(param_6);
      if (dVar31 <= dVar28) {
        ABIBQBZIL_HPgKFQryD(param_6);
      }
      lVar20 = MLMYqQy_HXSPAIPP(uVar13);
      uVar21 = MLMYqQy_HXSPAIPP(uVar14);
      lVar22 = MLMYqQy_HXSPAIPP(uVar15);
      lVar23 = PBQaLJMeOyMEnLFKQBOPXSPAIPP(param_1,param_3,lVar20,uVar21,lVar22,uVar16,local_40);
      lVar19 = 2;
      if (1 < DAT_00e8e600) {
        lVar19 = DAT_00e8e600;
      }
      if (lVar19 < local_40[0]) break;
      lVar27 = lVar27 + uVar21;
      lVar19 = param_2 + (lVar2 - lVar27) * 8;
      kkbdOLJkjb(param_6,uVar16,lVar19,lVar11);
      if (uVar21 == 0) goto LAB_0099fec8;
      uVar24 = 1;
      do {
        *(undefined8 *)(lVar19 + uVar24 * 8) =
             *(undefined8 *)(lVar23 + *(long *)(lVar19 + uVar24 * 8) * 8);
        uVar24 = uVar24 + 1;
      } while (uVar24 <= uVar21);
      lVar19 = BJMQVYqQy_HXSPAIPP(uVar13);
    }
    kjbdFKAnyOQFQFLKXSPAIPP(local_68,param_6,uVar16,uVar17,lVar11);
    lVar19 = kjbaLKSBOQcADBqBMrLlLABqBMXSPAIPP(param_6,uVar16,lVar11);
    if (lVar19 < 1) {
      lVar27 = lVar27 + uVar21;
      lVar19 = param_2 + (lVar2 - lVar27) * 8;
      kkbdOLJkjb(param_6,uVar16,lVar19,lVar11);
      if (uVar21 != 0) {
        uVar24 = 1;
        do {
          *(undefined8 *)(lVar19 + uVar24 * 8) =
               *(undefined8 *)(lVar23 + *(long *)(lVar19 + uVar24 * 8) * 8);
          uVar24 = uVar24 + 1;
        } while (uVar24 <= uVar21);
      }
    }
    else {
      lVar27 = lVar27 + lVar19;
      lVar1 = param_2 + (lVar2 - lVar27) * 8;
      _LMVgtB_QLOXSPAIPP(*(undefined8 *)(lVar11 + 0xb0),lVar19,lVar1);
      if (lVar18 >> 5 <= (long)uVar21) {
        ABIBQBZIL_HPgKFQryD(param_6);
      }
      lVar4 = *(long *)(lVar11 + 0x58);
      FKFQgtB_QLOXSPAIPP(lVar4,uVar21,0);
      uVar3 = *(undefined8 *)(lVar11 + 0x50);
      FKFQgtB_QLOXSPAIPP(uVar3,uVar21,0);
      lVar5 = *(long *)(lVar11 + 0x70);
      FKFQgtB_QLOXSPAIPP(lVar5,uVar21,0);
      uVar12 = *(undefined8 *)(lVar11 + 0x90);
      FKFQgtB_QLOXSPAIPP(uVar12,uVar21,0);
      lVar6 = *(long *)(lVar11 + 0x40);
      *(undefined8 *)(lVar6 + 0x10) = *(undefined8 *)(lVar11 + 0x98);
      FKFQaLRKQFKDkyOHXSPAIPP(uVar21,lVar6);
      lVar7 = *(long *)(lVar11 + 0x30);
      uVar8 = *(undefined8 *)(lVar11 + 0x60);
      uVar9 = *(undefined8 *)(lVar11 + 0xa8);
      uVar10 = *(undefined8 *)(lVar11 + 0x68);
      *(ulong *)(lVar7 + 0x10) = uVar21;
      *(undefined8 *)(lVar7 + 0x18) = uVar8;
      FKFQoXSPAIPP(lVar7);
      lVar19 = CFKAaLKKaLJMPYKAkyHBaLKQFDX2
                         (param_6,lVar19,lVar1,uVar16,lVar23,lVar4,uVar3,lVar5,uVar12,lVar2,uVar9,
                          uVar10,*(undefined8 *)(lVar6 + 0x10),lVar7);
      if (0 < lVar19) {
        plVar26 = (long *)(lVar4 + 8);
        plVar25 = plVar26 + lVar19;
        do {
          *plVar26 = *plVar26 + lVar20 + -1;
          plVar26 = plVar26 + 1;
        } while (plVar26 != plVar25);
        plVar26 = (long *)(lVar5 + 8);
        plVar25 = plVar26 + lVar19;
        do {
          *plVar26 = *plVar26 + lVar22 + -1;
          plVar26 = plVar26 + 1;
        } while (plVar26 != plVar25);
      }
      kjbnRPEaLJMLKBKQPXSPAIPP(lVar19,lVar4,uVar3,lVar5,uVar13,uVar14,uVar15);
      PBQaLRKQFKDkyOHrLlRIIXSPAIPP(lVar6);
      PBQorLlRIIXSPAIPP(lVar7);
    }
  } while( true );
}


