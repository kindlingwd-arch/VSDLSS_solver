/* driver: DBQkIALSPnyOQoXSPAIPP addr=00997120 size=90 */
#include "vsdlss_ref.h"


long DBQkIALSPnyOQoXSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  long lVar1;
  
  lVar1 = *(long *)(param_3 + param_2 * 8);
  *(undefined8 *)(lVar1 + 8) = param_1;
  *(undefined8 *)(lVar1 + 0x10) = 0;
  if (param_2 == 1) {
    *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_3 + 0x60);
    *(undefined8 *)(lVar1 + 0x20) = *(undefined8 *)(param_3 + 0x68);
    *(undefined8 *)(lVar1 + 0x28) = *(undefined8 *)(param_3 + 0x70);
    FKFQnNF(lVar1);
    return lVar1;
  }
  if (param_2 != 2) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0x2f6);
    FKFQnNF(lVar1);
    return lVar1;
  }
  *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_3 + 0x78);
  *(undefined8 *)(lVar1 + 0x20) = *(undefined8 *)(param_3 + 0x80);
  *(undefined8 *)(lVar1 + 0x28) = *(undefined8 *)(param_3 + 0x88);
  FKFQnNF(lVar1);
  return lVar1;
}


