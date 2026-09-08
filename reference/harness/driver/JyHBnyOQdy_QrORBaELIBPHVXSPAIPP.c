/* driver: JyHBnyOQdy_QrORBaELIBPHVXSPAIPP addr=0096e2f0 size=a8 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void JyHBnyOQdy_QrORBaELIBPHVXSPAIPP(long param_1,long param_2)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  double dVar6;
  double *pdVar7;
  long lVar9;
  double dVar10;
  double *pdVar8;
  
  dVar6 = _DAT_00ab67f8;
  lVar9 = 1;
  lVar2 = *(long *)(param_2 + 0x58);
  lVar3 = *(long *)(param_2 + 0x48);
  param_1 = *(long *)(param_2 + 0x10) - param_1;
  if (0 < param_1) {
    do {
      lVar4 = *(long *)(lVar2 + lVar9 * 8);
      lVar5 = *(long *)(lVar3 + lVar9 * 8);
      dVar10 = SQRT(*(double *)(lVar4 + 8));
      if (NAN(dVar10)) {
        dVar10 = sqrt(*(double *)(lVar4 + 8));
      }
      pdVar7 = (double *)(lVar4 + 8);
      pdVar1 = pdVar7 + lVar5;
      if (0 < lVar5) {
        do {
          pdVar8 = pdVar7 + 1;
          *pdVar7 = *pdVar7 * (dVar6 / dVar10);
          pdVar7 = pdVar8;
        } while (pdVar8 != pdVar1);
      }
      lVar9 = lVar9 + 1;
    } while (lVar9 <= param_1);
  }
  return;
}


