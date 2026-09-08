/* driver: zy_HTyOAqLISBZIL_HBAXSPAIPP addr=0096d6d0 size=50d */
#include "vsdlss_ref.h"


void zy_HTyOAqLISBZIL_HBAXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  double *pdVar2;
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
  long lVar14;
  double *pdVar15;
  long lVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  long local_58;
  double local_48;
  double local_40;
  
  local_58 = *(long *)(param_1 + 0x28);
  lVar3 = *(long *)(param_1 + 0x10);
  lVar4 = *(long *)(param_1 + 0x58);
  local_48 = 0.0;
  local_40 = 0.0;
  lVar5 = *(long *)(param_1 + 0x50);
  lVar6 = *(long *)(param_1 + 0x40);
  lVar7 = *(long *)(param_1 + 0x48);
  lVar11 = lVar3;
  if (0 < local_58) {
    do {
      lVar10 = *(long *)(lVar6 + local_58 * 8);
      switch(lVar10) {
      default:
        lVar12 = lVar11;
        if (0 < lVar10) {
          lVar13 = lVar10 + -1;
          lVar16 = 1;
          do {
            lVar14 = lVar13 + 1;
            if (5 < lVar13) {
              lVar14 = 6;
            }
            lVar12 = lVar11 - lVar14;
            lVar1 = lVar12 + 1;
            lVar8 = *(long *)(lVar7 + lVar1 * 8);
            lVar9 = (lVar3 - lVar1) + 1;
            if (lVar9 == lVar8) {
              zIL_HgKKBOnOLAR_QZy_HqLISBlgXSPAIPP
                        (lVar11,lVar14,lVar4 + -8 + lVar1 * 8,lVar9,param_2);
            }
            else {
              zIL_HgKKBOnOLAR_QZy_HqLISBXSPAIPP
                        (lVar11,lVar14,*(undefined8 *)(lVar5 + lVar1 * 8),lVar4 + -8 + lVar1 * 8,
                         lVar8,param_2);
            }
            lVar16 = lVar16 + 6;
            lVar13 = lVar13 + -6;
            lVar11 = lVar12;
          } while (lVar16 <= lVar10);
        }
        break;
      case 1:
        lVar13 = *(long *)(lVar4 + lVar11 * 8);
        lVar16 = *(long *)(lVar7 + lVar11 * 8);
        lVar10 = (lVar3 - lVar11) + 1;
        if (lVar10 == lVar16) {
          if (lVar10 < 2) goto LAB_0096dbe7;
          dVar18 = 0.0;
          lVar10 = 2;
          do {
            lVar16 = lVar10 * 8;
            lVar12 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar18 = dVar18 + *(double *)(param_2 + -8 + lVar11 * 8 + lVar16) *
                              *(double *)(lVar13 + lVar12);
          } while (lVar10 != (lVar3 - lVar11) + 2);
        }
        else if (lVar16 < 2) {
LAB_0096dbe7:
          dVar18 = 0.0;
        }
        else {
          dVar18 = 0.0;
          lVar10 = 2;
          do {
            lVar12 = lVar10 * 8;
            lVar14 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar18 = dVar18 + *(double *)
                               (param_2 + *(long *)(*(long *)(lVar5 + lVar11 * 8) + lVar12) * 8) *
                              *(double *)(lVar13 + lVar14);
          } while (lVar10 != lVar16 + 1);
        }
        pdVar15 = (double *)(lVar11 * 8 + param_2);
        lVar12 = lVar11 + -1;
        *pdVar15 = (*pdVar15 - dVar18) / *(double *)(lVar13 + 8);
        break;
      case 2:
        lVar13 = lVar11 * 8;
        lVar16 = *(long *)(lVar4 + lVar11 * 8);
        lVar14 = *(long *)(lVar4 + -8 + lVar13);
        lVar12 = *(long *)(lVar7 + lVar11 * 8);
        lVar10 = (lVar3 - lVar11) + 1;
        if (lVar10 == lVar12) {
          if (lVar10 < 2) goto LAB_0096dbda;
          dVar19 = 0.0;
          lVar10 = 2;
          dVar18 = 0.0;
          do {
            dVar20 = *(double *)(lVar13 + -8 + param_2 + lVar10 * 8);
            lVar1 = lVar10 * 8;
            lVar12 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar18 = dVar18 + *(double *)(lVar14 + 8 + lVar1) * dVar20;
            dVar19 = dVar19 + dVar20 * *(double *)(lVar16 + lVar12);
          } while (lVar10 != (lVar3 - lVar11) + 2);
        }
        else if (lVar12 < 2) {
LAB_0096dbda:
          dVar19 = 0.0;
          dVar18 = dVar19;
        }
        else {
          dVar19 = 0.0;
          lVar10 = 2;
          dVar18 = 0.0;
          do {
            lVar8 = lVar10 * 8;
            dVar20 = *(double *)
                      (param_2 + *(long *)(*(long *)(lVar5 + lVar11 * 8) + lVar10 * 8) * 8);
            lVar1 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar18 = dVar18 + *(double *)(lVar14 + 8 + lVar8) * dVar20;
            dVar19 = dVar19 + dVar20 * *(double *)(lVar16 + lVar1);
          } while (lVar10 != lVar12 + 1);
        }
        pdVar15 = (double *)(param_2 + -8 + lVar11 * 8);
        lVar12 = lVar11 + -2;
        dVar19 = (*(double *)(lVar13 + param_2) - dVar19) / *(double *)(lVar16 + 8);
        *(double *)(lVar13 + param_2) = dVar19;
        *pdVar15 = (*pdVar15 - (dVar19 * *(double *)(lVar14 + 0x10) + dVar18)) /
                   *(double *)(lVar14 + 8);
        break;
      case 3:
        lVar13 = lVar11 * 8;
        lVar16 = *(long *)(lVar4 + lVar11 * 8);
        lVar14 = *(long *)(lVar4 + -0x10 + lVar13);
        lVar1 = *(long *)(lVar4 + -8 + lVar13);
        lVar12 = *(long *)(lVar7 + lVar11 * 8);
        lVar10 = (lVar3 - lVar11) + 1;
        if (lVar10 == lVar12) {
          dVar18 = local_40;
          dVar19 = local_40;
          dVar20 = local_40;
          if (1 < lVar10) {
            dVar18 = 0.0;
            lVar10 = 2;
            dVar19 = 0.0;
            dVar20 = 0.0;
            do {
              dVar17 = *(double *)(lVar13 + -8 + param_2 + lVar10 * 8);
              dVar20 = dVar20 + *(double *)(lVar14 + 0x10 + lVar10 * 8) * dVar17;
              lVar8 = lVar10 * 8;
              lVar12 = lVar10 * 8;
              lVar10 = lVar10 + 1;
              dVar19 = dVar19 + *(double *)(lVar1 + 8 + lVar8) * dVar17;
              dVar18 = dVar18 + dVar17 * *(double *)(lVar16 + lVar12);
            } while (lVar10 != (lVar3 - lVar11) + 2);
          }
        }
        else if (lVar12 < 2) {
          dVar18 = 0.0;
          dVar19 = dVar18;
          dVar20 = dVar18;
        }
        else {
          lVar10 = 2;
          dVar18 = local_48;
          dVar19 = local_48;
          dVar20 = local_48;
          do {
            dVar17 = *(double *)
                      (param_2 + *(long *)(*(long *)(lVar5 + lVar11 * 8) + lVar10 * 8) * 8);
            dVar20 = dVar20 + *(double *)(lVar14 + 0x10 + lVar10 * 8) * dVar17;
            lVar9 = lVar10 * 8;
            lVar8 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar19 = dVar19 + *(double *)(lVar1 + 8 + lVar9) * dVar17;
            dVar18 = dVar18 + dVar17 * *(double *)(lVar16 + lVar8);
          } while (lVar10 != lVar12 + 1);
        }
        lVar10 = lVar11 * 8 + -8;
        lVar12 = lVar11 + -3;
        pdVar15 = (double *)(param_2 + lVar10);
        pdVar2 = (double *)(param_2 + -8 + lVar10);
        dVar17 = (*(double *)(lVar13 + param_2) - dVar18) / *(double *)(lVar16 + 8);
        *(double *)(lVar13 + param_2) = dVar17;
        dVar18 = *(double *)(lVar14 + 0x18);
        dVar19 = (*pdVar15 - (dVar17 * *(double *)(lVar1 + 0x10) + dVar19)) / *(double *)(lVar1 + 8)
        ;
        *pdVar15 = dVar19;
        *pdVar2 = (*pdVar2 - (dVar19 * *(double *)(lVar14 + 0x10) + dVar18 * dVar17 + dVar20)) /
                  *(double *)(lVar14 + 8);
        break;
      case 4:
      case 5:
      case 6:
        lVar12 = lVar11 - lVar10;
        lVar13 = lVar12 + 1;
        lVar16 = *(long *)(lVar7 + lVar13 * 8);
        lVar14 = (lVar3 - lVar13) + 1;
        if (lVar14 == lVar16) {
          zIL_HgKKBOnOLAR_QZy_HqLISBlgXSPAIPP(lVar11,lVar10,lVar4 + -8 + lVar13 * 8,lVar14,param_2);
        }
        else {
          zIL_HgKKBOnOLAR_QZy_HqLISBXSPAIPP
                    (lVar11,lVar10,*(undefined8 *)(lVar5 + lVar13 * 8),lVar4 + -8 + lVar13 * 8,
                     lVar16,param_2);
        }
      }
      local_58 = local_58 + -1;
      lVar11 = lVar12;
    } while (local_58 != 0);
  }
  return;
}


