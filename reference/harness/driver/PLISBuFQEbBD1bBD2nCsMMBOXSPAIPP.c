/* driver: PLISBuFQEbBD1bBD2nCsMMBOXSPAIPP addr=009a5630 size=cb */
#include "vsdlss_ref.h"


void PLISBuFQEbBD1bBD2nCsMMBOXSPAIPP(long param_1,long param_2,long param_3)

{
  double dVar1;
  double dVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  double dVar12;
  
  lVar11 = *(long *)(param_1 + 8);
  lVar3 = *(long *)(param_1 + 0x18);
  lVar4 = *(long *)(param_1 + 0x30);
  lVar5 = *(long *)(param_1 + 0x38);
  lVar6 = *(long *)(param_1 + 0x40);
  lVar10 = lVar11 + *(long *)(param_1 + 0x10);
  lVar7 = *(long *)(param_1 + 0x48);
  if (lVar11 < lVar10) {
    do {
      lVar8 = *(long *)(lVar4 + lVar10 * 8);
      dVar12 = *(double *)(param_2 + lVar10 * 8);
      dVar1 = *(double *)(lVar5 + lVar10 * 8);
      lVar9 = *(long *)(lVar6 + lVar10 * 8);
      dVar2 = *(double *)(lVar7 + lVar10 * 8);
      *(double *)(param_3 + lVar10 * 8) = dVar12;
      if (0 < lVar8) {
        dVar12 = dVar12 - dVar1 * *(double *)(param_3 + lVar8 * 8);
        *(double *)(param_3 + lVar10 * 8) = dVar12;
      }
      if (0 < lVar9) {
        dVar12 = dVar12 - dVar2 * *(double *)(param_3 + lVar9 * 8);
        *(double *)(param_3 + lVar10 * 8) = dVar12;
      }
      *(double *)(param_3 + lVar10 * 8) = dVar12 / *(double *)(lVar3 + lVar10 * 8);
      lVar10 = lVar10 + -1;
    } while (lVar10 != lVar11);
  }
  if (0 < lVar11) {
    do {
      lVar6 = *(long *)(lVar4 + lVar11 * 8);
      dVar12 = *(double *)(param_2 + lVar11 * 8);
      dVar1 = *(double *)(lVar5 + lVar11 * 8);
      *(double *)(param_3 + lVar11 * 8) = dVar12;
      if (0 < lVar6) {
        dVar12 = dVar12 - dVar1 * *(double *)(param_3 + lVar6 * 8);
        *(double *)(param_3 + lVar11 * 8) = dVar12;
      }
      *(double *)(param_3 + lVar11 * 8) = dVar12 / *(double *)(lVar3 + lVar11 * 8);
      lVar11 = lVar11 + -1;
    } while (lVar11 != 0);
  }
  return;
}


