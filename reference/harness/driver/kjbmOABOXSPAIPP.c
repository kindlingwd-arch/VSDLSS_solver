/* driver: kjbmOABOXSPAIPP addr=009a1fc0 size=216 */
#include "vsdlss_ref.h"


void kjbmOABOXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                    undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 *param_8,
                    long *param_9)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 local_e0;
  long local_d8;
  undefined8 local_d0;
  undefined1 local_c8 [152];
  
  if (mljwXkYrpgvXtYjscqXafYlecb == 0) {
    DBQkjbeOyMEdOLJcIBJYOOXSPAIPP();
    lVar1 = BKNbRJMkBQFP();
    if (lVar1 != 0) {
      ARJMkBQFP(local_c8);
      KOBOOLOXSPAIPP(0x39,"mldorder.c",0x1911);
    }
    FKFQgtB_QLOXSPAIPP(param_7,param_1,0xffffffffffffffff);
    local_e0 = 0;
    kjbmOABOgKQBOKyIXJQ(local_c8,param_5,param_6,param_7,&local_e0,*param_8,&local_d8);
    ABIBQBkjbeOyMElLfByABO(local_c8);
    kjb_EB_HdLOnOLMBOpBLOABOFKDXSPAIPP(param_1,local_e0,param_7);
  }
  else {
    lVar1 = DBQpBLOABOtB_nOBS();
    lVar3 = DBQdzPMFnOBS();
    _LMVgtB_QLOXSPAIPP(lVar1,param_1,param_7);
    if (lVar1 != 0) {
      _yPFdOBBXSPAIPP(lVar1);
    }
    PBQpBLOABOtB_nOBS(0);
    if (lVar3 == 0) {
      local_d8 = 0;
      local_d0 = 0;
      goto LAB_009a209e;
    }
    _LMVdzPMF(*param_8,lVar3,&local_d8);
    ABIBQBdzPMF(lVar3);
    PBQdzPMFnOBS(0);
  }
  if (local_d8 == 0) {
    local_d0 = 0;
  }
  else {
    _LMVdzPMF(*param_8,local_d8,&local_d0);
  }
LAB_009a209e:
  PBQdzPMFdOLJkjb(local_d0);
  DBQkjbeOyMEdOLJcIBJYOOXSPAIPP(param_1,param_2,param_3,param_4,local_c8);
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,0,0);
  param_8[8] = uVar2;
  kkbdOLJkjbqQRzlBTXSPAIPP(local_c8,param_5,param_6,param_7,param_8);
  ABIBQBkjbeOyMElLfByABO(local_c8);
  *param_9 = local_d8;
  return;
}


