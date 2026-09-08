/* driver: DBQkIALSPZMNXSPAIPP addr=009b60c0 size=2a */
#include "vsdlss_ref.h"


long DBQkIALSPZMNXSPAIPP(undefined8 param_1,long param_2)

{
  long lVar1;
  
  lVar1 = *(long *)(param_2 + 0x38);
  *(undefined8 *)(lVar1 + 0x30) = *(undefined8 *)(param_2 + 0x60);
  *(undefined8 *)(lVar1 + 0x38) = *(undefined8 *)(param_2 + 0x68);
  *(undefined8 *)(lVar1 + 0x40) = *(undefined8 *)(param_2 + 0x70);
  FKFQZMNXSPAIPP(param_1,lVar1);
  return lVar1;
}


