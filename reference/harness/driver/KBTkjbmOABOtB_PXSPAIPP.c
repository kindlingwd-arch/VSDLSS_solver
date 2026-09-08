/* driver: KBTkjbmOABOtB_PXSPAIPP addr=00996bc0 size=180 */
#include "vsdlss_ref.h"


long KBTkjbmOABOtB_PXSPAIPP(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  
  lVar4 = param_1 * 2 + 4;
  param_1 = param_1 + 1;
  lVar1 = _yPFkyIIL_XSPAIPP(0xb8);
  uVar2 = KBTnNFfByABOmKIVXSPAIPP();
  *(undefined8 *)(lVar1 + 8) = uVar2;
  uVar2 = KBTnNFfByABOmKIVXSPAIPP();
  *(undefined8 *)(lVar1 + 0x10) = uVar2;
  uVar2 = KBTfByMfByABOmKIV();
  *(undefined8 *)(lVar1 + 0x20) = uVar2;
  uVar2 = KBTfByMfByABOmKIV();
  *(undefined8 *)(lVar1 + 0x28) = uVar2;
  uVar2 = KBTofByABOmKIVXSPAIPP();
  *(undefined8 *)(lVar1 + 0x30) = uVar2;
  uVar2 = KBTZMNfByABOmKIVXSPAIPP();
  *(undefined8 *)(lVar1 + 0x38) = uVar2;
  uVar2 = KBTaLRKQFKDkyOHfByABOmKIVXSPAIPP();
  *(undefined8 *)(lVar1 + 0x40) = uVar2;
  uVar2 = KBTatB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x48) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x50) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x58) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x60) = uVar2;
  lVar3 = KBTgtB_QLOXSPAIPP(lVar4,1,0);
  *(long *)(lVar1 + 0x68) = lVar3;
  *(long *)(lVar1 + 0x70) = lVar3 + param_1 * 8;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x78) = uVar2;
  lVar4 = KBTgtB_QLOXSPAIPP(lVar4,1,0);
  *(long *)(lVar1 + 0x88) = param_1 * 8 + lVar4;
  *(long *)(lVar1 + 0x80) = lVar4;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x90) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0x98) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0xa0) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0xa8) = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar1 + 0xb0) = uVar2;
  return lVar1;
}


