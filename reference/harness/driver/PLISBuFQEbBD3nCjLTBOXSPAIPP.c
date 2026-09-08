/* driver: PLISBuFQEbBD3nCjLTBOXSPAIPP addr=009a5150 size=d4 */
#include "vsdlss_ref.h"


void PLISBuFQEbBD3nCjLTBOXSPAIPP(long param_1,long param_2,long param_3)

{
  double *pdVar1;
  double dVar2;
  double dVar3;
  double dVar4;
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
  double dVar17;
  
  lVar16 = 1;
  lVar5 = *(long *)(param_1 + 8);
  lVar6 = *(long *)(param_1 + 0x10);
  lVar7 = *(long *)(param_1 + 0x18);
  lVar8 = *(long *)(param_1 + 0x20);
  lVar9 = *(long *)(param_1 + 0x28);
  lVar10 = *(long *)(param_1 + 0x30);
  lVar11 = *(long *)(param_1 + 0x38);
  lVar12 = *(long *)(param_1 + 0x40);
  if (0 < lVar5) {
    do {
      lVar13 = *(long *)(lVar7 + lVar16 * 8);
      dVar2 = *(double *)(lVar8 + lVar16 * 8);
      lVar14 = *(long *)(lVar9 + lVar16 * 8);
      dVar17 = *(double *)(param_2 + lVar16 * 8) / *(double *)(lVar6 + lVar16 * 8);
      lVar15 = *(long *)(lVar11 + lVar16 * 8);
      dVar3 = *(double *)(lVar10 + lVar16 * 8);
      dVar4 = *(double *)(lVar12 + lVar16 * 8);
      *(double *)(param_3 + lVar16 * 8) = dVar17;
      if (0 < lVar13) {
        pdVar1 = (double *)(param_2 + lVar13 * 8);
        *pdVar1 = *pdVar1 - dVar17 * dVar2;
      }
      if (0 < lVar14) {
        pdVar1 = (double *)(param_2 + lVar14 * 8);
        *pdVar1 = *pdVar1 - dVar3 * *(double *)(param_3 + lVar16 * 8);
      }
      if (0 < lVar15) {
        pdVar1 = (double *)(param_2 + lVar15 * 8);
        *pdVar1 = *pdVar1 - dVar4 * *(double *)(param_3 + lVar16 * 8);
      }
      lVar16 = lVar16 + 1;
    } while (lVar16 != lVar5 + 1);
  }
  return;
}


