/* driver: PLISBjLyAayPBXSPAIPP addr=0096cff0 size=1c0 */
#include "vsdlss_ref.h"


void PLISBjLyAayPBXSPAIPP(long param_1,ulong *param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  ulong *puVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double *pdVar14;
  long lVar15;
  double dVar16;
  
  uVar8 = DAT_00abd200;
  lVar12 = *(long *)(param_1 + 0x10);
  lVar3 = *(long *)(param_1 + 0x50);
  lVar4 = *(long *)(param_1 + 0x58);
  lVar5 = *(long *)(param_1 + 0x48);
  if (0 < lVar12) {
    lVar13 = 1;
    lVar15 = lVar12;
    puVar7 = param_2;
    do {
      lVar11 = *(long *)(lVar5 + lVar13 * 8);
      lVar10 = *(long *)(lVar4 + lVar13 * 8);
      dVar16 = (double)(puVar7[1] ^ uVar8) / *(double *)(lVar10 + 8);
      if (lVar15 == lVar11) {
        lVar11 = 2;
        if (1 < lVar15) {
          do {
            puVar7[lVar11] =
                 (ulong)(*(double *)(lVar10 + lVar11 * 8) * dVar16 + (double)puVar7[lVar11]);
            lVar11 = lVar11 + 1;
          } while (lVar11 != (lVar12 + 2) - lVar13);
        }
      }
      else {
        lVar6 = *(long *)(lVar3 + lVar13 * 8);
        if (1 < lVar11) {
          lVar9 = 2;
          do {
            lVar1 = lVar9 * 8;
            lVar2 = lVar9 * 8;
            lVar9 = lVar9 + 1;
            param_2[*(long *)(lVar6 + lVar1)] =
                 (ulong)(*(double *)(lVar10 + lVar2) * dVar16 +
                        (double)param_2[*(long *)(lVar6 + lVar1)]);
          } while (lVar9 != lVar11 + 1);
        }
      }
      lVar13 = lVar13 + 1;
      lVar15 = lVar15 + -1;
      puVar7 = puVar7 + 1;
    } while (lVar13 != lVar12 + 1);
    lVar15 = 1;
    pdVar14 = (double *)(param_2 + lVar12);
    do {
      lVar13 = *(long *)(lVar5 + lVar12 * 8);
      lVar11 = *(long *)(lVar4 + lVar12 * 8);
      dVar16 = 0.0;
      if (lVar15 == lVar13) {
        if (lVar15 == 1) {
          lVar15 = 2;
        }
        else {
          lVar15 = lVar15 + 1;
          dVar16 = 0.0;
          lVar13 = 2;
          do {
            lVar6 = lVar13 + -1;
            lVar10 = lVar13 * 8;
            lVar13 = lVar13 + 1;
            dVar16 = dVar16 + pdVar14[lVar6] * *(double *)(lVar11 + lVar10);
          } while (lVar13 != lVar15);
        }
      }
      else {
        if (1 < lVar13) {
          dVar16 = 0.0;
          lVar10 = 2;
          do {
            lVar6 = lVar10 * 8;
            lVar9 = lVar10 * 8;
            lVar10 = lVar10 + 1;
            dVar16 = dVar16 + (double)param_2[*(long *)(*(long *)(lVar3 + lVar12 * 8) + lVar6)] *
                              *(double *)(lVar11 + lVar9);
          } while (lVar10 != lVar13 + 1);
        }
        lVar15 = lVar15 + 1;
      }
      *pdVar14 = (*pdVar14 - dVar16) / *(double *)(lVar11 + 8);
      lVar12 = lVar12 + -1;
      pdVar14 = pdVar14 + -1;
    } while (lVar12 != 0);
  }
  return;
}


