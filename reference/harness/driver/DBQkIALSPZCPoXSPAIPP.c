/* driver: DBQkIALSPZCPoXSPAIPP addr=009970e0 size=1e */
#include "vsdlss_ref.h"


long DBQkIALSPZCPoXSPAIPP(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_2 + 0x30);
  uVar2 = *(undefined8 *)(param_2 + 0x60);
  *(undefined8 *)(lVar1 + 0x10) = param_1;
  *(undefined8 *)(lVar1 + 0x18) = uVar2;
  FKFQoXSPAIPP(lVar1);
  return lVar1;
}


