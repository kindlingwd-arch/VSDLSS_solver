/* driver: PKFMbFPMacADBIFPQcIBJXSPAIPP addr=009742a0 size=3d */
#include "vsdlss_ref.h"


undefined8 PKFMbFPMacADBIFPQcIBJXSPAIPP(undefined8 *param_1,long param_2,long param_3)

{
  long lVar1;
  
  if (param_3 != 0) {
    *(undefined8 *)(param_3 + 8) = *(undefined8 *)(param_2 + 8);
    lVar1 = param_2;
    *(long *)(param_2 + 8) = DAT_00e74ed8;
    DAT_00e74ed8 = lVar1;
    return *(undefined8 *)(param_3 + 8);
  }
  *param_1 = *(undefined8 *)(param_2 + 8);
  lVar1 = param_2;
  *(long *)(param_2 + 8) = DAT_00e74ed8;
  DAT_00e74ed8 = lVar1;
  return *param_1;
}


