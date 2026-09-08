/* driver: PLISBuFQEbBD1bBD2nCjLTBOXSPAIPP addr=009a4f80 size=e4 */
#include "vsdlss_ref.h"


void PLISBuFQEbBD1bBD2nCjLTBOXSPAIPP(long param_1,long param_2,long param_3)

{
  double *pdVar1;
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
  double dVar13;
  
  lVar10 = 1;
  lVar3 = *(long *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 0x18);
  lVar5 = *(long *)(param_1 + 0x30);
  lVar6 = *(long *)(param_1 + 0x38);
  lVar7 = *(long *)(param_1 + 0x40);
  lVar11 = lVar3 + *(long *)(param_1 + 0x10);
  lVar8 = *(long *)(param_1 + 0x48);
  lVar9 = lVar3 + 1;
  if (0 < lVar3) {
    do {
      lVar3 = *(long *)(lVar5 + lVar10 * 8);
      dVar2 = *(double *)(lVar6 + lVar10 * 8);
      dVar12 = *(double *)(param_2 + lVar10 * 8) / *(double *)(lVar4 + lVar10 * 8);
      *(double *)(param_3 + lVar10 * 8) = dVar12;
      if (0 < lVar3) {
        pdVar1 = (double *)(param_2 + lVar3 * 8);
        *pdVar1 = *pdVar1 - dVar12 * dVar2;
      }
      lVar10 = lVar10 + 1;
    } while (lVar10 != lVar9);
  }
  if (lVar9 <= lVar11) {
    do {
      lVar3 = *(long *)(lVar5 + lVar9 * 8);
      dVar2 = *(double *)(lVar6 + lVar9 * 8);
      lVar10 = *(long *)(lVar7 + lVar9 * 8);
      dVar13 = *(double *)(param_2 + lVar9 * 8) / *(double *)(lVar4 + lVar9 * 8);
      dVar12 = *(double *)(lVar8 + lVar9 * 8);
      *(double *)(param_3 + lVar9 * 8) = dVar13;
      if (0 < lVar3) {
        pdVar1 = (double *)(param_2 + lVar3 * 8);
        *pdVar1 = *pdVar1 - dVar13 * dVar2;
      }
      if (0 < lVar10) {
        pdVar1 = (double *)(param_2 + lVar10 * 8);
        *pdVar1 = *pdVar1 - dVar12 * *(double *)(param_3 + lVar9 * 8);
      }
      lVar9 = lVar9 + 1;
    } while (lVar9 != lVar11 + 1);
  }
  return;
}


