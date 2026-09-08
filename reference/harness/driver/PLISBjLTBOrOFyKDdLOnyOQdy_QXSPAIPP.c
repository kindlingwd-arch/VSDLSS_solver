/* driver: PLISBjLTBOrOFyKDdLOnyOQdy_QXSPAIPP addr=0096cef0 size=f4 */
#include "vsdlss_ref.h"


void PLISBjLTBOrOFyKDdLOnyOQdy_QXSPAIPP(long param_1,long param_2,double *param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  double *pdVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  double *pdVar13;
  long lVar14;
  long lVar15;
  double dVar16;
  
  uVar10 = DAT_00abd200;
  lVar15 = *(long *)(param_2 + 0x10);
  lVar4 = *(long *)(param_2 + 0x50);
  lVar5 = *(long *)(param_2 + 0x58);
  lVar6 = *(long *)(param_2 + 0x48);
  param_1 = lVar15 - param_1;
  if (0 < param_1) {
    lVar1 = lVar15 + 2;
    lVar14 = 1;
    pdVar9 = param_3;
    do {
      pdVar13 = pdVar9 + 1;
      lVar7 = *(long *)(lVar5 + lVar14 * 8);
      lVar12 = *(long *)(lVar6 + lVar14 * 8);
      dVar16 = *pdVar13 / *(double *)(lVar7 + 8);
      *pdVar13 = dVar16;
      dVar16 = (double)((ulong)dVar16 ^ uVar10);
      if (lVar15 == lVar12) {
        lVar12 = 2;
        if (1 < lVar15) {
          do {
            pdVar9[lVar12] = *(double *)(lVar7 + lVar12 * 8) * dVar16 + pdVar9[lVar12];
            lVar12 = lVar12 + 1;
          } while (lVar12 != lVar1 - lVar14);
        }
      }
      else {
        lVar8 = *(long *)(lVar4 + lVar14 * 8);
        if (1 < lVar12) {
          lVar11 = 2;
          do {
            lVar2 = lVar11 * 8;
            lVar3 = lVar11 * 8;
            lVar11 = lVar11 + 1;
            param_3[*(long *)(lVar8 + lVar3)] =
                 *(double *)(lVar7 + lVar2) * dVar16 + param_3[*(long *)(lVar8 + lVar3)];
          } while (lVar11 != lVar12 + 1);
        }
      }
      lVar14 = lVar14 + 1;
      lVar15 = lVar15 + -1;
      pdVar9 = pdVar13;
    } while (lVar14 != param_1 + 1);
  }
  return;
}


