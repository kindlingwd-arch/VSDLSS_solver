/* driver: _LJMRQBdy_QLOZIL_HkBJXSPAIPP addr=009904d0 size=93 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

double _LJMRQBdy_QLOZIL_HkBJXSPAIPP(double param_1,long param_2,long param_3,long param_4)

{
  long *plVar1;
  long *plVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  
  if (param_2 < 1) {
    dVar5 = 0.0;
    dVar4 = _DAT_00b1a140;
  }
  else {
    dVar4 = 0.0;
    plVar2 = (long *)(param_3 + 8);
    plVar1 = plVar2 + param_2;
    dVar5 = 0.0;
    do {
      dVar3 = (double)*plVar2 * DAT_00ab6798;
      plVar2 = plVar2 + 1;
      if (dVar5 <= dVar3) {
        dVar5 = dVar3;
      }
      if (dVar4 <= dVar3) {
        dVar4 = dVar3;
      }
    } while (plVar2 != plVar1);
    dVar4 = dVar4 + DAT_00ab6798 + DAT_00ab6798;
  }
  dVar4 = ((double)param_4 * dVar4 + dVar5) * DAT_00ab7248;
  if ((double)param_2 * _DAT_00b1a140 <= dVar4) {
    dVar4 = (double)param_2 * _DAT_00b1a140;
  }
  if (dVar4 <= param_1 * DAT_00abc268) {
    dVar4 = param_1 * DAT_00abc268;
  }
  return dVar4;
}


