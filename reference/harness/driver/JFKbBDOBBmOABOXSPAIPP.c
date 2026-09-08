/* driver: JFKbBDOBBmOABOXSPAIPP addr=009923b0 size=dc */
#include "vsdlss_ref.h"


void JFKbBDOBBmOABOXSPAIPP
               (undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4)

{
  long *plVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  long lVar7;
  
  plVar1 = (long *)*param_1;
  lVar7 = *plVar1;
  uVar2 = KBTgtB_QLOXSPAIPP(lVar7,1,0xffffffff8000000b);
  uVar3 = KBTgtB_QLOXSPAIPP(lVar7,1,0);
  uVar4 = KBTgKAFPQlLABqBQPXSPAIPP(lVar7);
  if (999 < lVar7) {
    lVar7 = lVar7 >> 2;
  }
  uVar5 = KBTfyPEryzIBXSPAIPP(lVar7);
  FKFQgKAFPQlLABqBQPXSPAIPP(uVar4);
  JFKbBDOBBmOABOZMN(plVar1,uVar2,uVar3,uVar4,uVar5);
  uVar6 = ABIBQBeOyMEXSPAIPP(plVar1);
  FKFQmOABOqBIB_QBAgKnOLzXSPAIPP = 0;
  ABIBQBfyPEryzIBXSPAIPP(uVar5);
  *param_1 = uVar6;
  *param_2 = uVar2;
  *param_3 = uVar3;
  *param_4 = uVar4;
  return;
}


