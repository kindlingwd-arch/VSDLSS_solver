/* driver: PRzZIL_HaLKQOFzdOLJpLTpyKDBmIAXSPAIPP addr=0097f880 size=811 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRzZIL_HaLKQOFzdOLJpLTpyKDBmIAXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6,
               long param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  long lVar6;
  double *pdVar7;
  long *plVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  ulong uVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  undefined8 uVar24;
  long lVar25;
  long lVar26;
  double dVar27;
  long local_8f8;
  long local_8e0;
  long local_8d0;
  long local_858 [130];
  double local_448 [131];
  
  lVar9 = *(long *)(param_6 + 0x48);
  param_2 = param_2 - param_1;
  lVar10 = *(long *)(lVar9 + param_1 * 8);
  lVar11 = *(long *)(param_6 + 0x50);
  lVar12 = *(long *)(param_6 + 0x58);
  lVar13 = *(long *)(lVar11 + param_1 * 8);
  lVar1 = param_2 + 2;
  lVar2 = param_2 + 1;
  if (lVar1 <= lVar10) {
    lVar23 = *(long *)(lVar13 + lVar1 * 8);
    lVar20 = lVar1;
    while (lVar23 < param_3) {
      lVar20 = lVar20 + 1;
      if (lVar10 < lVar20) {
        return;
      }
      lVar23 = *(long *)(lVar13 + lVar20 * 8);
    }
    if (0 < lVar2) {
      lVar18 = 1;
      do {
        local_448[lVar18] =
             _DAT_00ab67f8 / *(double *)(*(long *)(param_1 * 8 + lVar12 + -8 + lVar18 * 8) + 8);
        lVar18 = lVar18 + 1;
      } while (lVar18 != lVar1);
    }
    lVar18 = lVar20 * 8;
    lVar26 = 0;
    lVar25 = lVar20;
    do {
      if (0 < lVar2) {
        lVar19 = 1;
        lVar21 = lVar18;
        do {
          puVar5 = (undefined8 *)(*(long *)(param_1 * 8 + lVar12 + -8 + lVar19 * 8) + lVar21);
          lVar21 = lVar21 + -8;
          *(undefined8 *)(param_10 + lVar26 * 8 + lVar19 * 8) = *puVar5;
          lVar19 = lVar19 + 1;
        } while (lVar19 != lVar1);
        lVar26 = param_2 + 1 + lVar26;
      }
      uVar15 = DAT_00abd200;
      lVar25 = lVar25 + 1;
      lVar18 = lVar18 + 8;
    } while (lVar25 <= lVar10);
    lVar12 = lVar1 * 8 + -8;
    local_8f8 = lVar20;
    while (lVar23 <= param_4) {
      lVar18 = *(long *)(param_6 + 0x80);
      if ((lVar18 == 0) || (lVar23 <= *(long *)(param_6 + 0x88))) {
        local_858[1] = *(undefined8 *)(*(long *)(param_6 + 0x58) + lVar23 * 8);
      }
      else {
        local_858[1] = *(undefined8 *)(lVar18 + (lVar23 - *(long *)(param_6 + 0x88)) * 8);
      }
      local_8d0 = local_8f8 + 1;
      local_8e0 = local_8d0 * 8;
      if (lVar10 < local_8d0) {
        lVar23 = 0;
        lVar26 = 1;
      }
      else {
        lVar25 = *(long *)(lVar13 + local_8d0 * 8);
        if ((param_4 < lVar25) ||
           (lVar21 = *(long *)(param_5 + lVar25 * 8), lVar21 != *(long *)(param_5 + lVar23 * 8))) {
          lVar23 = 0;
          lVar26 = 1;
        }
        else {
          lVar26 = 1;
          do {
            lVar26 = lVar26 + 1;
            if ((lVar18 == 0) || (lVar25 <= *(long *)(param_6 + 0x88))) {
              lVar23 = *(long *)(*(long *)(param_6 + 0x58) + lVar25 * 8);
            }
            else {
              lVar23 = *(long *)(lVar18 + (lVar25 - *(long *)(param_6 + 0x88)) * 8);
            }
            local_858[lVar26] = lVar23;
            local_8d0 = local_8f8 + lVar26;
          } while (((local_8d0 <= lVar10) &&
                   (lVar25 = *(long *)(lVar13 + -8 + local_8e0 + lVar26 * 8), lVar25 <= param_4)) &&
                  (*(long *)(param_5 + lVar25 * 8) == lVar21));
          local_8e0 = local_8d0 * 8;
          lVar23 = 1 - lVar26;
        }
      }
      lVar21 = 1;
      lVar25 = 0;
      lVar18 = param_10 + (local_8f8 - lVar20) * lVar2 * 8;
      do {
        if (0 < lVar2) {
          lVar19 = 1;
          do {
            *(double *)(lVar25 * 8 + param_9 + lVar19 * 8) =
                 (double)(*(ulong *)(lVar18 + lVar25 * 8 + lVar19 * 8) ^ uVar15) * local_448[lVar19]
            ;
            lVar19 = lVar19 + 1;
          } while (lVar19 != lVar1);
          lVar25 = param_2 + 1 + lVar25;
        }
        lVar21 = lVar21 + 1;
      } while (lVar21 <= lVar26);
      if (lVar2 < 6) {
        if (lVar26 != 1) {
          lVar25 = lVar13 + local_8f8 * 8;
          lVar19 = 1;
          lVar21 = lVar18;
          do {
            lVar14 = *(long *)(lVar25 + -8 + lVar19 * 8);
            lVar22 = 1;
            lVar17 = param_9;
            do {
              lVar6 = lVar22 * 8;
              dVar27 = 0.0;
              if (0 < lVar2) {
                lVar16 = 1;
                do {
                  lVar3 = lVar16 * 8;
                  lVar4 = lVar16 * 8;
                  lVar16 = lVar16 + 1;
                  dVar27 = dVar27 + *(double *)(lVar17 + lVar3) * *(double *)(lVar21 + lVar4);
                } while (lVar16 != lVar1);
              }
              lVar17 = lVar17 + lVar12;
              plVar8 = local_858 + lVar22;
              lVar22 = lVar22 + 1;
              pdVar7 = (double *)(*plVar8 + 8 + (lVar14 - *(long *)(lVar25 + -8 + lVar6)) * 8);
              *pdVar7 = dVar27 + *pdVar7;
            } while (lVar22 <= lVar19);
            lVar19 = lVar19 + 1;
            lVar21 = lVar21 + lVar12;
          } while (lVar19 != lVar26);
          lVar18 = lVar18 + (lVar26 + -1) * lVar12;
          goto LAB_0097fc5d;
        }
LAB_0097fef7:
        lVar25 = *(long *)(lVar13 + -8 + local_8e0);
      }
      else {
        if (((param_7 == 0) ||
            (((double)lVar26 - _DAT_00ab67f8) * (double)lVar26 * (double)lVar2 * DAT_00ab7228 <
             _DAT_00b1a158)) || (lVar26 < 6)) {
          RMAyQBjByArOFbBPQqRMlLABpyKDB
                    (local_8f8,lVar2,lVar13,1,lVar26 + -1,local_858,param_9,lVar18);
          lVar18 = lVar18 + (lVar26 + -1) * lVar2 * 8;
          if (lVar26 == 1) goto LAB_0097fef7;
        }
        else {
          RMAyQBjByArOFbBPQqRMlLABnyO(local_8f8,lVar2,lVar13,lVar26,local_858,param_9,lVar18);
          lVar18 = lVar18 + (lVar26 + -1) * lVar2 * 8;
        }
LAB_0097fc5d:
        lVar25 = *(long *)(lVar13 + -8 + local_8e0);
        lVar21 = 1;
        do {
          local_858[lVar21] =
               local_858[lVar21] +
               (lVar25 - *(long *)(lVar13 + local_8f8 * 8 + -8 + lVar21 * 8)) * 8;
          lVar21 = lVar21 + 1;
        } while (lVar21 < lVar26);
      }
      lVar23 = lVar23 + 1 + (lVar10 - local_8f8);
      lVar21 = *(long *)(lVar9 + lVar25 * 8);
      if (0 < lVar23) {
        uVar24 = 0;
        if (lVar23 != lVar21) {
          DBQqByO_EgKAF_BPXSPAIPP
                    (lVar13 + -0x10 + (local_8f8 + lVar26) * 8,lVar23,
                     *(undefined8 *)(lVar11 + lVar25 * 8),lVar21,param_8);
          uVar24 = param_8;
        }
        if (lVar2 < 7) {
          JyQkyQkRIQqO_cUQjco6X_yPFXSPAIPP(param_9,lVar23,lVar2,lVar18,lVar26,param_11);
          yAArLbBPQtB_PXSPAIPP(lVar23,uVar24,lVar21,lVar26,param_11,local_858);
        }
        else if ((param_7 == 0) || ((double)lVar23 * (double)lVar2 * (double)lVar26 < _DAT_00b1a158)
                ) {
          JyQkyQkRIQX_yPFXSPAIPP(param_9,lVar23,lVar2,lVar18,lVar26,param_11,1);
          yAArLbBPQtB_PZVaLIPXSPAIPP(lVar23,uVar24,lVar21,lVar26,param_11,local_858);
        }
        else {
          JyQkyQkRIQX_yPFnyOyIIBI(param_9,lVar23,lVar2,lVar18,lVar26,param_11,1,uVar24,local_858);
        }
      }
      local_8f8 = local_8d0;
      if (lVar10 < local_8d0) {
        return;
      }
      lVar23 = *(long *)(lVar13 + local_8e0);
    }
  }
  return;
}


