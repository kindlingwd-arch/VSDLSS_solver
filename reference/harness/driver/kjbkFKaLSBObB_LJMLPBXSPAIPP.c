/* driver: kjbkFKaLSBObB_LJMLPBXSPAIPP addr=0099b820 size=181 */
#include "vsdlss_ref.h"


long kjbkFKaLSBObB_LJMLPBXSPAIPP
               (long param_1,undefined8 param_2,undefined8 param_3,long param_4,long param_5,
               undefined8 param_6)

{
  long lVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long alStack_78 [9];
  
  FKFQgtB_QLOXSPAIPP(alStack_78,6,0);
  FKFQgtB_QLOXSPAIPP(param_5,*(undefined8 *)(param_1 + 0x10),5);
  FKFQgtB_QLOXSPAIPP(param_5,*(undefined8 *)(param_1 + 8),2);
  kjbnBOCLOJZdqdLOqBQPXSPAIPP(8,param_1,param_2,param_3,param_5,param_6);
  kjbnBOCLOJZdqdLOqBQPXSPAIPP(7,param_1,param_2,param_3,param_5,param_6);
  lVar3 = *(long *)(param_1 + 0x10);
  if (0 < lVar3) {
    lVar6 = 1;
    do {
      while (lVar4 = *(long *)(param_5 + lVar6 * 8), 5 < lVar4 - 1U) {
        lVar6 = lVar6 + 1;
        KOBOOLOXSPAIPP(1,"mldorder.c",0xd09);
        lVar3 = *(long *)(param_1 + 0x10);
        alStack_78[lVar4] = alStack_78[lVar4] + 1;
        if (lVar3 < lVar6) goto LAB_0099b8fc;
      }
      lVar6 = lVar6 + 1;
      alStack_78[lVar4] = alStack_78[lVar4] + 1;
    } while (lVar6 <= lVar3);
  }
LAB_0099b8fc:
  uVar2 = (alStack_78[4] + alStack_78[5]) - alStack_78[3];
  uVar7 = (long)uVar2 >> 0x3f;
  lVar4 = (uVar2 ^ uVar7) - uVar7;
  lVar6 = (alStack_78[4] - alStack_78[2]) - alStack_78[3];
  if (lVar6 < 1) {
    lVar6 = alStack_78[3] + (alStack_78[2] - alStack_78[4]);
  }
  if (lVar3 < 1) {
    lVar3 = 0;
  }
  else {
    lVar5 = 1;
    lVar3 = 0;
    do {
      lVar1 = *(long *)(param_5 + lVar5 * 8);
      if (((lVar1 - 3U < 2) || ((lVar1 == 5 && (lVar4 < lVar6)))) ||
         ((lVar1 == 2 && (lVar6 <= lVar4)))) {
        lVar3 = lVar3 + 1;
        *(long *)(param_4 + lVar3 * 8) = lVar5;
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 <= *(long *)(param_1 + 0x10));
  }
  return lVar3;
}


