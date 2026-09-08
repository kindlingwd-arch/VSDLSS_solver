/* driver: JFKbBDOBBmOABOYOOXSPAIPP addr=00995c30 size=1fc */
#include "vsdlss_ref.h"


void JFKbBDOBBmOABOYOOXSPAIPP
               (long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,long *param_5,
               undefined8 param_6,long param_7)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40 [2];
  
  lVar1 = *param_5;
  lVar5 = 1;
  if (0x3ff < lVar1) {
    lVar5 = lVar1 >> 10;
  }
  lVar2 = KBTatB_QLOXSPAIPP(lVar1,1,0);
  lVar3 = KBTgtB_QLOXSPAIPP(lVar1,0,0);
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  local_40[0] = 0;
  FKFQgtB_QLOXSPAIPP(param_6,lVar1,0xffffffff8000000c);
  FKFQgtB_QLOXSPAIPP(lVar3,lVar1,0);
  if (param_1 == 0) {
    JFKbBDOBBmOABO123mKIV
              (param_5,lVar5,&local_58,&local_50,&local_48,local_40,param_6,param_7,lVar2,lVar3);
  }
  JFKbBDOBBmOABOZMNYOO
            (param_1,param_2,param_3,param_4,lVar5,param_5,&local_58,&local_50,&local_48,local_40,
             param_6,param_7,lVar2,lVar3);
  if (lVar3 != 0) {
    _yPFdOBBXSPAIPP(lVar3);
  }
  *(undefined8 *)(param_7 + 0x30) = 1;
  *(undefined8 *)(param_7 + 0x38) = local_40[0];
  *(undefined8 *)(param_7 + 0x28) = local_40[0];
  uVar4 = DBQZIL_HcUQBKQPdOLJZIL_HkyOHtB_(lVar1,lVar2);
  *(undefined8 *)(param_7 + 0x40) = uVar4;
  if (lVar2 != 0) {
    _yPFdOBBXSPAIPP(lVar2);
  }
  FKFQmOABOqBIB_QBAgKnOLzXSPAIPP = 0;
  return;
}


