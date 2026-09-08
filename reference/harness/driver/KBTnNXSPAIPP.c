/* driver: KBTnNXSPAIPP addr=009a68b0 size=77 */
#include "vsdlss_ref.h"


undefined8 * KBTnNXSPAIPP(undefined8 param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  uVar1 = _OByQBryDXSPAIPP();
  puVar2 = (undefined8 *)KBTZIL_HXSPAIPP(uVar1,0x30);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar2[2] = 0;
  uVar3 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,1,0xffffffffffffffff);
  puVar2[3] = uVar3;
  uVar3 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,0,0);
  puVar2[4] = uVar3;
  uVar1 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,0,0);
  puVar2[5] = uVar1;
  return puVar2;
}


