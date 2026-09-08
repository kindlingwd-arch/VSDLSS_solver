/* driver: PLISBuFQEbBD3nCsMMBOXSPAIPP addr=009a54d0 size=ab */
#include "vsdlss_ref.h"


void PLISBuFQEbBD3nCsMMBOXSPAIPP(long param_1,long param_2,long param_3)

{
  double dVar1;
  double dVar2;
  double dVar3;
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
  double dVar15;
  
  lVar14 = *(long *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 0x10);
  lVar5 = *(long *)(param_1 + 0x18);
  lVar6 = *(long *)(param_1 + 0x20);
  lVar7 = *(long *)(param_1 + 0x28);
  lVar8 = *(long *)(param_1 + 0x30);
  lVar9 = *(long *)(param_1 + 0x38);
  lVar10 = *(long *)(param_1 + 0x40);
  if (0 < lVar14) {
    do {
      lVar11 = *(long *)(lVar5 + lVar14 * 8);
      dVar15 = *(double *)(param_2 + lVar14 * 8);
      dVar1 = *(double *)(lVar6 + lVar14 * 8);
      lVar12 = *(long *)(lVar7 + lVar14 * 8);
      dVar2 = *(double *)(lVar8 + lVar14 * 8);
      lVar13 = *(long *)(lVar9 + lVar14 * 8);
      dVar3 = *(double *)(lVar10 + lVar14 * 8);
      *(double *)(param_3 + lVar14 * 8) = dVar15;
      if (0 < lVar11) {
        dVar15 = dVar15 - dVar1 * *(double *)(param_3 + lVar11 * 8);
        *(double *)(param_3 + lVar14 * 8) = dVar15;
      }
      if (0 < lVar12) {
        dVar15 = dVar15 - dVar2 * *(double *)(param_3 + lVar12 * 8);
        *(double *)(param_3 + lVar14 * 8) = dVar15;
      }
      if (0 < lVar13) {
        dVar15 = dVar15 - dVar3 * *(double *)(param_3 + lVar13 * 8);
        *(double *)(param_3 + lVar14 * 8) = dVar15;
      }
      *(double *)(param_3 + lVar14 * 8) = dVar15 / *(double *)(lVar4 + lVar14 * 8);
      lVar14 = lVar14 + -1;
    } while (lVar14 != 0);
  }
  return;
}


