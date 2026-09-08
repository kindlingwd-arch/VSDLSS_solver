/* driver: PRzZIL_HaLKQOFzdOLJpLTpyKDBX1XSPAIPP addr=009800a0 size=73f */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRzZIL_HaLKQOFzdOLJpLTpyKDBX1XSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6,
               long param_7,undefined8 param_8,long param_9,long param_10,undefined8 param_11)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long *plVar4;
  long *plVar5;
  double *pdVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  undefined8 uVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  double dVar24;
  long local_8f8;
  long local_8e8;
  long local_8c8;
  long local_858 [130];
  double local_448 [131];
  
  uVar13 = DAT_00abd200;
  lVar7 = *(long *)(param_6 + 0x48);
  lVar8 = *(long *)(param_6 + 0x50);
  param_2 = param_2 - param_1;
  lVar9 = *(long *)(lVar7 + param_1 * 8);
  lVar10 = *(long *)(param_6 + 0x58);
  lVar11 = *(long *)(lVar8 + param_1 * 8);
  lVar1 = param_2 + 1;
  if (param_3 <= lVar1) {
    param_3 = param_2 + 2;
  }
  if (lVar9 <= param_4) {
    param_4 = lVar9;
  }
  if ((param_3 <= lVar9) && (param_3 <= param_4)) {
    if (0 < lVar1) {
      lVar14 = 1;
      do {
        local_448[lVar14] =
             _DAT_00ab67f8 / *(double *)(*(long *)(lVar10 + param_1 * 8 + -8 + lVar14 * 8) + 8);
        lVar14 = lVar14 + 1;
      } while (lVar14 != param_2 + 2);
    }
    lVar10 = lVar1 * 8;
    lVar14 = param_3;
    do {
      lVar22 = lVar14 * 8;
      plVar4 = (long *)(lVar11 + lVar22);
      lVar23 = *plVar4;
      lVar18 = *(long *)(param_6 + 0x80);
      if ((lVar18 == 0) || (lVar23 <= *(long *)(param_6 + 0x88))) {
        local_858[1] = *(undefined8 *)(*(long *)(param_6 + 0x58) + lVar23 * 8);
      }
      else {
        local_858[1] = *(undefined8 *)(lVar18 + (lVar23 - *(long *)(param_6 + 0x88)) * 8);
      }
      local_8c8 = lVar14 + 1;
      if (param_4 < local_8c8) {
        local_8f8 = lVar22 + 8;
        local_8e8 = 0;
        lVar22 = 1;
      }
      else {
        local_8f8 = lVar22 + 8;
        lVar21 = *(long *)(lVar11 + 8 + lVar22);
        lVar15 = *(long *)(param_5 + lVar21 * 8);
        if (lVar15 == *(long *)(param_5 + lVar23 * 8)) {
          lVar22 = 1;
          do {
            lVar22 = lVar22 + 1;
            if ((lVar18 == 0) || (lVar21 <= *(long *)(param_6 + 0x88))) {
              local_858[lVar22] = *(long *)(*(long *)(param_6 + 0x58) + lVar21 * 8);
            }
            else {
              local_858[lVar22] = *(long *)(lVar18 + (lVar21 - *(long *)(param_6 + 0x88)) * 8);
            }
          } while ((lVar14 + lVar22 <= param_4) &&
                  (lVar21 = plVar4[lVar22], *(long *)(param_5 + lVar21 * 8) == lVar15));
          local_8c8 = lVar14 + lVar22;
          local_8e8 = 1 - lVar22;
          local_8f8 = local_8c8 * 8;
        }
        else {
          local_8e8 = 0;
          lVar22 = 1;
        }
      }
      lVar21 = 1;
      lVar18 = 0;
      lVar23 = param_10 + (lVar14 - param_3) * lVar1 * 8;
      do {
        if (0 < lVar1) {
          lVar15 = 1;
          do {
            *(double *)(lVar18 * 8 + param_9 + lVar15 * 8) =
                 (double)(*(ulong *)(lVar23 + lVar18 * 8 + lVar15 * 8) ^ uVar13) * local_448[lVar15]
            ;
            lVar15 = lVar15 + 1;
          } while (lVar15 != param_2 + 2);
          lVar18 = param_2 + 1 + lVar18;
        }
        lVar21 = lVar21 + 1;
      } while (lVar21 <= lVar22);
      if (lVar1 < 6) {
        if (lVar22 != 1) {
          lVar21 = 1;
          lVar18 = lVar23;
          do {
            lVar15 = plVar4[lVar21 + -1];
            lVar20 = 1;
            lVar19 = param_9;
            do {
              lVar12 = lVar20 + -1;
              dVar24 = 0.0;
              if (0 < lVar1) {
                lVar16 = 1;
                do {
                  lVar2 = lVar16 * 8;
                  lVar3 = lVar16 * 8;
                  lVar16 = lVar16 + 1;
                  dVar24 = dVar24 + *(double *)(lVar19 + lVar2) * *(double *)(lVar18 + lVar3);
                } while (lVar16 != param_2 + 2);
              }
              lVar19 = lVar19 + lVar10;
              plVar5 = local_858 + lVar20;
              lVar20 = lVar20 + 1;
              pdVar6 = (double *)(*plVar5 + 8 + (lVar15 - plVar4[lVar12]) * 8);
              *pdVar6 = dVar24 + *pdVar6;
            } while (lVar20 <= lVar21);
            lVar21 = lVar21 + 1;
            lVar18 = lVar18 + lVar10;
          } while (lVar21 != lVar22);
          lVar23 = lVar23 + (lVar22 + -1) * lVar10;
          goto LAB_009803ed;
        }
LAB_00980655:
        lVar18 = *(long *)(lVar11 + -8 + local_8f8);
      }
      else {
        if (((param_7 == 0) ||
            (((double)lVar22 - _DAT_00ab67f8) * (double)lVar22 * (double)lVar1 * DAT_00ab7228 <
             _DAT_00b1a158)) || (lVar22 < 6)) {
          RMAyQBjByArOFbBPQqRMlLABpyKDB(lVar14,lVar1,lVar11,1,lVar22 + -1,local_858,param_9,lVar23);
          lVar23 = lVar23 + (lVar22 + -1) * lVar1 * 8;
          if (lVar22 == 1) goto LAB_00980655;
        }
        else {
          RMAyQBjByArOFbBPQqRMlLABnyO(lVar14,lVar1,lVar11,lVar22,local_858,param_9,lVar23);
          lVar23 = lVar23 + (lVar22 + -1) * lVar1 * 8;
        }
LAB_009803ed:
        lVar18 = *(long *)(lVar11 + -8 + local_8f8);
        lVar21 = 1;
        do {
          local_858[lVar21] = local_858[lVar21] + (lVar18 - plVar4[lVar21 + -1]) * 8;
          lVar21 = lVar21 + 1;
        } while (lVar21 < lVar22);
      }
      lVar21 = local_8e8 + 1 + (lVar9 - lVar14);
      lVar15 = *(long *)(lVar7 + lVar18 * 8);
      if (0 < lVar21) {
        uVar17 = 0;
        if (lVar21 != lVar15) {
          DBQqByO_EgKAF_BPXSPAIPP
                    (lVar11 + -0x10 + (lVar14 + lVar22) * 8,lVar21,
                     *(undefined8 *)(lVar8 + lVar18 * 8),lVar15,param_8);
          uVar17 = param_8;
        }
        if (lVar1 < 7) {
          JyQkyQkRIQqO_cUQjco6X_yPFXSPAIPP(param_9,lVar21,lVar1,lVar23,lVar22,param_11);
          yAArLbBPQtB_PXSPAIPP(lVar21,uVar17,lVar15,lVar22,param_11,local_858);
        }
        else if ((param_7 == 0) || ((double)lVar21 * (double)lVar1 * (double)lVar22 < _DAT_00b1a158)
                ) {
          JyQkyQkRIQX_yPFXSPAIPP(param_9,lVar21,lVar1,lVar23,lVar22,param_11,1);
          yAArLbBPQtB_PZVaLIPXSPAIPP(lVar21,uVar17,lVar15,lVar22,param_11,local_858);
        }
        else {
          JyQkyQkRIQX_yPFnyOyIIBI(param_9,lVar21,lVar1,lVar23,lVar22,param_11,1,uVar17,local_858);
        }
      }
      lVar14 = local_8c8;
    } while (local_8c8 <= param_4);
  }
  return;
}


