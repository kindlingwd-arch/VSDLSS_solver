/* driver: OBJdOLJbBNXSPAIPP addr=009b2760 size=6b */
#include "vsdlss_ref.h"


undefined8 OBJdOLJbBNXSPAIPP(undefined8 *param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  
  puVar3 = (undefined8 *)param_1[1];
  puVar4 = (undefined8 *)param_1[2];
  puVar1 = (undefined8 *)puVar3[1];
  if (puVar3 != puVar4) {
    if (puVar1 == (undefined8 *)0x0) {
      KOBOOLOXSPAIPP(1,"dequtils.c",0x5c);
      puVar3 = (undefined8 *)param_1[1];
      puVar4 = (undefined8 *)param_1[2];
    }
    uVar2 = *puVar1;
    puVar3[1] = puVar1[1];
    if (puVar1 == puVar4) {
      param_1[2] = puVar3;
    }
    ABIBQBacADBIFPQXSPAIPP(puVar1);
    return uVar2;
  }
  return *param_1;
}


