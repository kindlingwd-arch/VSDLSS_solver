/* driver: CLOTyOAqLISBZIL_HBAXSPAIPP addr=0096d1c0 size=4e7 */
#include "vsdlss_ref.h"


void CLOTyOAqLISBZIL_HBAXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  double *pdVar3;
  double *pdVar4;
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
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  long local_78;
  
  lVar5 = *(long *)(param_1 + 0x50);
  lVar6 = *(long *)(param_1 + 0x10);
  local_78 = 1;
  lVar7 = *(long *)(param_1 + 0x58);
  lVar8 = *(long *)(param_1 + 0x28);
  lVar9 = *(long *)(param_1 + 0x40);
  lVar10 = *(long *)(param_1 + 0x48);
  lVar15 = 1;
  if (0 < lVar8) {
    do {
      lVar11 = *(long *)(lVar9 + local_78 * 8);
      switch(lVar11) {
      default:
        lVar16 = lVar11 + -1;
        if (0 < lVar11) {
          lVar18 = 1;
          do {
            lVar12 = 6;
            if (lVar16 < 6) {
              lVar12 = lVar16 + 1;
            }
            lVar13 = *(long *)(lVar10 + lVar15 * 8);
            lVar14 = (lVar6 - lVar15) + 1;
            if (lVar14 == lVar13) {
              zIL_HqyUMVZy_HqLISBlgXSPAIPP(lVar15,lVar12,lVar7 + -8 + lVar15 * 8,lVar14,param_2);
            }
            else {
              zIL_HqyUMVZy_HqLISBXSPAIPP
                        (lVar15,lVar12,*(undefined8 *)(lVar5 + lVar15 * 8),lVar7 + -8 + lVar15 * 8,
                         lVar13,param_2);
            }
            lVar15 = lVar15 + lVar12;
            lVar18 = lVar18 + 6;
            lVar16 = lVar16 + -6;
          } while (lVar18 <= lVar11);
        }
        break;
      case 1:
        lVar16 = *(long *)(lVar7 + lVar15 * 8);
        lVar18 = *(long *)(lVar10 + lVar15 * 8);
        lVar11 = (lVar6 - lVar15) + 1;
        dVar19 = (double)(*(ulong *)(param_2 + lVar15 * 8) ^ DAT_00abd200) / *(double *)(lVar16 + 8)
        ;
        if (lVar11 == lVar18) {
          lVar18 = param_2 + -8 + lVar15 * 8;
          lVar12 = 2;
          if (1 < lVar11) {
            do {
              *(double *)(lVar18 + lVar12 * 8) =
                   *(double *)(lVar16 + lVar12 * 8) * dVar19 + *(double *)(lVar18 + lVar12 * 8);
              lVar12 = lVar12 + 1;
            } while (lVar12 != (lVar6 - lVar15) + 2);
            lVar15 = lVar15 + 1;
            break;
          }
        }
        else {
          lVar11 = *(long *)(lVar5 + lVar15 * 8);
          if (1 < lVar18) {
            lVar12 = 2;
            do {
              pdVar3 = (double *)(param_2 + *(long *)(lVar11 + lVar12 * 8) * 8);
              lVar13 = lVar12 * 8;
              lVar12 = lVar12 + 1;
              *pdVar3 = *(double *)(lVar16 + lVar13) * dVar19 + *pdVar3;
            } while (lVar12 != lVar18 + 1);
          }
        }
        lVar15 = lVar15 + 1;
        break;
      case 2:
        lVar16 = lVar15 * 8;
        lVar18 = *(long *)(lVar7 + lVar15 * 8);
        pdVar3 = (double *)(param_2 + 8 + lVar16);
        lVar12 = *(long *)(lVar7 + 8 + lVar16);
        dVar19 = (double)(*(ulong *)(param_2 + lVar15 * 8) ^ DAT_00abd200) / *(double *)(lVar18 + 8)
        ;
        dVar20 = *(double *)(lVar18 + 0x10) * dVar19 + *pdVar3;
        *pdVar3 = dVar20;
        lVar13 = *(long *)(lVar10 + 8 + lVar16);
        lVar14 = lVar6 - (lVar15 + 1);
        lVar11 = lVar14 + 1;
        dVar20 = (double)((ulong)dVar20 ^ DAT_00abd200) / *(double *)(lVar12 + 8);
        if (lVar11 == lVar13) {
          lVar13 = 2;
          if (1 < lVar11) {
            do {
              *(double *)(lVar16 + param_2 + lVar13 * 8) =
                   *(double *)(lVar18 + 8 + lVar13 * 8) * dVar19 +
                   *(double *)(lVar12 + lVar13 * 8) * dVar20 +
                   *(double *)(lVar16 + param_2 + lVar13 * 8);
              lVar13 = lVar13 + 1;
            } while (lVar13 != lVar14 + 2);
            lVar15 = lVar15 + 2;
            break;
          }
        }
        else {
          lVar11 = *(long *)(lVar5 + 8 + lVar16);
          if (1 < lVar13) {
            lVar16 = 2;
            do {
              pdVar3 = (double *)(param_2 + *(long *)(lVar11 + lVar16 * 8) * 8);
              lVar17 = lVar16 * 8;
              lVar14 = lVar16 * 8;
              lVar16 = lVar16 + 1;
              *pdVar3 = *(double *)(lVar18 + 8 + lVar17) * dVar19 +
                        *(double *)(lVar12 + lVar14) * dVar20 + *pdVar3;
            } while (lVar16 != lVar13 + 1);
          }
        }
        lVar15 = lVar15 + 2;
        break;
      case 3:
        lVar16 = lVar15 * 8;
        lVar18 = *(long *)(lVar7 + lVar15 * 8);
        pdVar3 = (double *)(param_2 + 8 + lVar16);
        lVar12 = *(long *)(lVar7 + 8 + lVar16);
        pdVar4 = (double *)(param_2 + 0x10 + lVar16);
        dVar19 = (double)(*(ulong *)(param_2 + lVar15 * 8) ^ DAT_00abd200) / *(double *)(lVar18 + 8)
        ;
        dVar20 = *(double *)(lVar18 + 0x10) * dVar19 + *pdVar3;
        *pdVar3 = dVar20;
        lVar13 = *(long *)(lVar7 + 0x10 + lVar16);
        dVar20 = (double)((ulong)dVar20 ^ DAT_00abd200) / *(double *)(lVar12 + 8);
        lVar17 = lVar6 - (lVar15 + 2);
        lVar11 = lVar17 + 1;
        dVar21 = *(double *)(lVar18 + 0x18) * dVar19 + *(double *)(lVar12 + 0x10) * dVar20 + *pdVar4
        ;
        *pdVar4 = dVar21;
        lVar14 = *(long *)(lVar10 + 0x10 + lVar16);
        dVar21 = (double)((ulong)dVar21 ^ DAT_00abd200) / *(double *)(lVar13 + 8);
        if (lVar11 == lVar14) {
          lVar16 = 2;
          if (1 < lVar11) {
            do {
              pdVar3[lVar16] =
                   *(double *)(lVar18 + 0x10 + lVar16 * 8) * dVar19 +
                   *(double *)(lVar12 + 8 + lVar16 * 8) * dVar20 +
                   *(double *)(lVar13 + lVar16 * 8) * dVar21 + pdVar3[lVar16];
              lVar16 = lVar16 + 1;
            } while (lVar16 != lVar17 + 2);
            lVar15 = lVar15 + 3;
            break;
          }
        }
        else {
          lVar11 = *(long *)(lVar5 + 0x10 + lVar16);
          if (1 < lVar14) {
            lVar16 = 2;
            do {
              pdVar3 = (double *)(param_2 + *(long *)(lVar11 + lVar16 * 8) * 8);
              lVar17 = lVar16 * 8;
              lVar2 = lVar16 * 8;
              lVar1 = lVar16 * 8;
              lVar16 = lVar16 + 1;
              *pdVar3 = *(double *)(lVar18 + 0x10 + lVar17) * dVar19 +
                        *(double *)(lVar12 + 8 + lVar2) * dVar20 +
                        *(double *)(lVar13 + lVar1) * dVar21 + *pdVar3;
            } while (lVar16 != lVar14 + 1);
          }
        }
        lVar15 = lVar15 + 3;
        break;
      case 4:
      case 5:
      case 6:
        lVar16 = *(long *)(lVar10 + lVar15 * 8);
        lVar18 = (lVar6 - lVar15) + 1;
        if (lVar18 == lVar16) {
          zIL_HqyUMVZy_HqLISBlgXSPAIPP(lVar15,lVar11,lVar7 + -8 + lVar15 * 8,lVar18,param_2);
        }
        else {
          zIL_HqyUMVZy_HqLISBXSPAIPP
                    (lVar15,lVar11,*(undefined8 *)(lVar5 + lVar15 * 8),lVar7 + -8 + lVar15 * 8,
                     lVar16,param_2);
        }
        lVar15 = lVar15 + lVar11;
      }
      local_78 = local_78 + 1;
    } while (local_78 <= lVar8);
  }
  return;
}


