/* driver: QyDlBTkjbeOyMEXSPAIPP addr=00996960 size=9d */
#include "vsdlss_ref.h"


void QyDlBTkjbeOyMEXSPAIPP(undefined8 param_1,undefined8 param_2,long param_3,long param_4)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  
  lVar1 = param_3 + 1;
  puVar3 = (undefined8 *)
           KBTZIL_HXSPAIPP(param_1,((param_4 + param_3) * 4 + 6 + lVar1 * 2) * 8 + 0x98);
  puVar4 = puVar3 + 0x13;
  puVar3[3] = param_4;
  *puVar3 = param_1;
  puVar3[1] = param_2;
  puVar3[0xc] = puVar4;
  puVar4 = puVar4 + lVar1;
  puVar3[0xe] = puVar4;
  puVar4 = puVar4 + lVar1;
  puVar3[2] = param_3;
  puVar3[10] = puVar4;
  puVar4 = puVar4 + lVar1;
  puVar3[0xb] = puVar4;
  puVar4 = puVar4 + lVar1;
  puVar2 = puVar4 + param_4 * 2 + 1;
  puVar3[0xd] = puVar4;
  puVar3[0xf] = puVar2;
  puVar3[0x10] = puVar2 + param_4 * 2 + 1;
  puVar3[0x11] = puVar2 + param_4 * 2 + 1 + lVar1;
  return;
}


