/* driver: kjbaLyOPBKmKBjBSBIXSPAIPP addr=00998ec0 size=19e */
#include "vsdlss_ref.h"


void kjbaLyOPBKmKBjBSBIXSPAIPP
               (undefined8 param_1,undefined8 param_2,long param_3,long param_4,long param_5)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  
  lVar1 = *(long *)(param_4 + param_3 * 8);
  uVar2 = *(undefined8 *)(param_5 + 0x58);
  uVar3 = *(undefined8 *)(param_5 + 0x60);
  uVar4 = *(undefined8 *)(param_5 + 0x48);
  lVar5 = *(long *)(lVar1 + 0x10);
  uVar6 = *(undefined8 *)(param_5 + 0x50);
  uVar7 = *(undefined8 *)(param_5 + 0x90);
  uVar8 = *(undefined8 *)(param_5 + 0x98);
  *(undefined8 *)(param_5 + 0x50) = 0;
  *(undefined8 *)(param_5 + 0x58) = 0;
  *(undefined8 *)(param_5 + 0x90) = 0;
  *(undefined8 *)(param_5 + 0x98) = 0;
  *(undefined8 *)(param_5 + 0x48) = 0;
  *(undefined8 *)(param_5 + 0x60) = 0;
  FKFQjyzBItB_XSPAIPP(uVar2,lVar5);
  if (4 < lVar5) {
    FUN_00995e30(uVar2,lVar5);
  }
  if (param_3 == 1) {
    uVar9 = kjbqfckkyQ_EFKDX1(param_2,lVar1,uVar6,uVar2,uVar7,uVar8);
    uVar9 = kjbaOByQBaLyOPBeOyMEX1(param_1,uVar9,lVar1,uVar6,uVar7,uVar4,uVar3,uVar8);
  }
  else {
    uVar9 = kjbqfckkyQ_EFKDXSPAIPP();
    uVar9 = kjbaOByQBaLyOPBeOyMEXSPAIPP(param_1,uVar9,lVar1,uVar6,uVar7,uVar4,uVar3,uVar8);
  }
  *(undefined8 *)(param_5 + 0x58) = uVar2;
  *(undefined8 *)(param_5 + 0x50) = uVar6;
  *(undefined8 *)(param_5 + 0x90) = uVar7;
  *(undefined8 *)(param_5 + 0x98) = uVar8;
  *(undefined8 *)(param_5 + 0x48) = uVar4;
  *(undefined8 *)(param_5 + 0x60) = uVar3;
  *(undefined8 *)(param_4 + 8 + param_3 * 8) = uVar9;
  return;
}


