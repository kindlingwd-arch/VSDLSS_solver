/* driver: KBTZIL_HgKCLXSPAIPP addr=0096ef10 size=86 */
#include "vsdlss_ref.h"


undefined8 * KBTZIL_HgKCLXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x40);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[2] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[3] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[4] = uVar2;
  uVar2 = KBTjgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[5] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[6] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,0,0);
  puVar1[7] = uVar2;
  return puVar1;
}


