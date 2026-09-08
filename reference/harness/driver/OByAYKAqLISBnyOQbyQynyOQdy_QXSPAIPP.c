/* driver: OByAYKAqLISBnyOQbyQynyOQdy_QXSPAIPP addr=009a2cf0 size=155 */
#include "vsdlss_ref.h"


void OByAYKAqLISBnyOQbyQynyOQdy_QXSPAIPP(char *param_1,long param_2,long param_3)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  uint *puVar6;
  uint *puVar7;
  undefined4 *puVar8;
  uint local_838 [256];
  uint local_438 [258];
  
  strcpy((char *)local_838,param_1);
  puVar7 = local_838;
  do {
    puVar6 = puVar7;
    uVar2 = *puVar6 + 0xfefefeff & ~*puVar6;
    uVar3 = uVar2 & 0x80808080;
    puVar7 = puVar6 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar7 = (uint *)((long)puVar6 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar8 = (undefined4 *)((long)puVar7 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar8 = 0x7261702e;
  *(undefined2 *)(puVar8 + 1) = 0x66;
  puVar4 = (undefined8 *)OByAnyOQdy_QXSPAIPP(local_838);
  lVar1 = puVar4[1];
  strcpy((char *)local_438,param_1);
  puVar7 = local_438;
  do {
    puVar6 = puVar7;
    uVar2 = *puVar6 + 0xfefefeff & ~*puVar6;
    uVar3 = uVar2 & 0x80808080;
    puVar7 = puVar6 + 1;
  } while (uVar3 == 0);
  if ((uVar2 & 0x8080) == 0) {
    puVar7 = (uint *)((long)puVar6 + 6);
    uVar3 = uVar3 >> 0x10;
  }
  puVar8 = (undefined4 *)((long)puVar7 + (-3 - (ulong)CARRY1((byte)uVar3,(byte)uVar3)));
  *puVar8 = 0x7265702e;
  *(undefined2 *)(puVar8 + 1) = 0x6d;
  puVar5 = (undefined8 *)KBTgtB_QLOXSPAIPP(lVar1 + param_2,0,0,uVar3 & 0xffffff00);
  *puVar5 = 0;
  OByAgtB_dOLJdFIBXSPAIPP(local_438,lVar1 + param_2,puVar5);
  if (param_3 == 0) {
    PLISBjLTBOrOFyKDnyOQdy_QXSPAIPP(param_1,param_2,puVar4,puVar5);
  }
  else if (param_3 == 1) {
    PLISBsMMBOrOFyKDnyOQdy_QXSPAIPP(param_1,param_2,puVar4,puVar5);
  }
  ABIBQBryDXSPAIPP(*puVar4);
  _yPFdOBBXSPAIPP(puVar5);
  return;
}


