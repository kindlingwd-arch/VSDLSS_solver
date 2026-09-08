/* driver: KBTbBD1bBD2nClLbFyDtB_XSPAIPP addr=00975660 size=a2 */
#include "vsdlss_ref.h"


undefined8 * KBTbBD1bBD2nClLbFyDtB_XSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  
  puVar3 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x50);
  puVar3[1] = param_2;
  param_2 = param_2 + param_3 * 2;
  *puVar3 = param_1;
  puVar3[2] = param_3;
  puVar3[3] = 0;
  puVar4 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_2,1,0);
  puVar3[4] = puVar4;
  *puVar4 = 0;
  puVar4 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_2,1);
  lVar1 = puVar3[2];
  lVar2 = puVar3[4];
  puVar3[5] = puVar4;
  *puVar4 = 0;
  puVar3[7] = puVar4;
  puVar3[6] = lVar2;
  puVar3[9] = puVar4 + lVar1;
  puVar3[8] = lVar2 + lVar1 * 8;
  return puVar3;
}


