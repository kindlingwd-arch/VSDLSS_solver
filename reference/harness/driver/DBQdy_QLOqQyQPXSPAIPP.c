/* driver: DBQdy_QLOqQyQPXSPAIPP addr=0096ec00 size=210 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DBQdy_QLOqQyQPXSPAIPP
               (long param_1,long *param_2,double *param_3,double *param_4,double *param_5,
               double *param_6,double *param_7,double *param_8,double *param_9,double *param_10)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  double dVar9;
  double dVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  
  dVar10 = DAT_00ab7228;
  dVar9 = _DAT_00ab67f8;
  dVar15 = DAT_00ab6798;
  lVar1 = param_2[1];
  lVar2 = *(long *)(param_1 + 0x10);
  lVar3 = *param_2;
  if (0 < lVar2) {
    lVar4 = *(long *)(param_1 + 0x48);
    lVar5 = *(long *)(param_1 + 0x58);
    lVar13 = 1;
    do {
      lVar12 = *(long *)(lVar4 + lVar13 * 8);
      lVar6 = *(long *)(lVar5 + lVar13 * 8);
      dVar19 = 0.0;
      if (lVar12 <= lVar6) {
        lVar7 = param_2[3];
        lVar8 = param_2[4];
        dVar19 = 0.0;
        do {
          dVar17 = (double)*(long *)(lVar7 + lVar12 * 8);
          dVar18 = (double)*(long *)(lVar8 + lVar12 * 8);
          dVar16 = dVar18 * dVar17 - (dVar17 - dVar9) * dVar17 * dVar10;
          dVar14 = dVar16 * dVar15;
          *param_8 = dVar16 + *param_8;
          dVar19 = dVar19 + dVar18 * dVar15 + dVar14;
          *param_5 = dVar18 * dVar15 + *param_5;
          *param_6 = dVar14 + *param_6;
          if (dVar9 <= dVar17) {
            dVar14 = *param_9;
            lVar11 = 1;
            dVar16 = dVar9;
            do {
              lVar11 = lVar11 + 1;
              dVar14 = dVar14 + ((dVar18 - dVar16) + dVar9) * (dVar18 - dVar16);
              dVar16 = (double)lVar11;
            } while (dVar16 <= dVar17);
            *param_9 = dVar14;
          }
          lVar12 = lVar12 + 1;
        } while (lVar12 != lVar6 + 1);
      }
      lVar13 = lVar13 + 1;
      dVar14 = *param_3;
      if (*param_3 <= dVar19) {
        dVar14 = dVar19;
      }
      *param_3 = dVar14;
      *param_4 = dVar19 + *param_4;
    } while (lVar13 != lVar2 + 1);
  }
  *param_7 = *param_5 + *param_6 + (double)lVar1 * dVar15 + (double)lVar1 * dVar15 +
             dVar15 * (double)lVar3 + *param_7;
  dVar15 = (*param_8 + *param_8) - (double)lVar3;
  *param_10 = dVar15 + dVar15;
  *param_4 = *param_4 / (double)lVar2;
  return;
}


