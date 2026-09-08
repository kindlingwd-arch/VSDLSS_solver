/* VSDLSS function (decompiled by Ghidra) */
/* name: minDegreeOrderArr_vsdlss  addr: 00a25f70  size: 1e3 */
#include "vsdlss_ref.h"

/* signature: void minDegreeOrderArr_vsdlss(int param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,int *param_5,undefined8 param_6,long param_7); */

void minDegreeOrderArr_vsdlss
               (int param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,int *param_5,
               undefined8 param_6,long param_7)

{
  int iVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  int iVar6;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40 [2];
  
  iVar1 = *param_5;
  lVar5 = (long)iVar1;
  iVar6 = 1;
  if (0x3ff < iVar1) {
    iVar6 = iVar1 >> 10;
  }
  lVar2 = newCVector_vsdlss(lVar5,1,0);
  lVar3 = newIVector_vsdlss(lVar5,0,0);
  local_4c = 0;
  local_40[0] = 0;
  local_48 = 0;
  local_44 = 0;
  initIVector_vsdlss(param_6,lVar5,0x8000000c);
  initIVector_vsdlss(lVar3,lVar5,0);
  if (param_1 == 0) {
    minDegreeOrder123Only
              (param_5,iVar6,&local_4c,local_40,&local_48,&local_44,param_6,param_7,lVar2,lVar3);
  }
  minDegreeOrderBpqArr
            (param_1,param_2,param_3,param_4,iVar6,param_5,&local_4c,local_40,&local_48,&local_44,
             param_6,param_7,lVar2,lVar3);
  if (lVar3 != 0) {
    casiFree_vsdlss(lVar3);
  }
  *(undefined4 *)(param_7 + 0x18) = 1;
  *(undefined4 *)(param_7 + 0x1c) = local_44;
  *(undefined4 *)(param_7 + 0x14) = local_44;
  uVar4 = getBlockExtentsFromBlockMarkVec(iVar1,lVar2);
  *(undefined8 *)(param_7 + 0x20) = uVar4;
  if (lVar2 != 0) {
    casiFree_vsdlss(lVar2);
  }
  initOrderSelectedInProb_vsdlss = 0;
  return;
}


