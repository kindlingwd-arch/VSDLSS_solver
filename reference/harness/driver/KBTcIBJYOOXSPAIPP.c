/* driver: KBTcIBJYOOXSPAIPP addr=00978ca0 size=8e */
#include "vsdlss_ref.h"


undefined8 * KBTcIBJYOOXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x50);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  *(undefined1 *)(puVar1 + 2) = 0;
  puVar1[3] = 0;
  *(undefined1 *)(puVar1 + 4) = 0;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  puVar1[5] = uVar2;
  puVar1[6] = 0;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  puVar1[7] = uVar2;
  uVar2 = KBTdtB_QLOXSPAIPP(0,param_1,1);
  puVar1[8] = uVar2;
  uVar2 = KBTdtB_QLOXSPAIPP(0,param_2,1);
  puVar1[9] = uVar2;
  return puVar1;
}


