/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDMinCoverDecompose_vsdlss  addr: 00a015d0  size: 18c */
#include "vsdlss_ref.h"

/* signature: int MLDMinCoverDecompose_vsdlss(long param_1,undefined8 param_2,undefined8 param_3,long param_4,int *param_5,undefined8 param_6); */

int MLDMinCoverDecompose_vsdlss
              (long param_1,undefined8 param_2,undefined8 param_3,long param_4,int *param_5,
              undefined8 param_6)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  uint uVar8;
  int aiStack_58 [4];
  int local_48;
  int local_44;
  
  initIVector_vsdlss(aiStack_58,6,0);
  initIVector_vsdlss(param_5,(long)*(int *)(param_1 + 8),5);
  initIVector_vsdlss(param_5,(long)*(int *)(param_1 + 4),2);
  MLDPerformBFSForSets_vsdlss(8,param_1,param_2,param_3,param_5,param_6);
  MLDPerformBFSForSets_vsdlss(7,param_1,param_2,param_3,param_5,param_6);
  iVar3 = *(int *)(param_1 + 8);
  if (0 < iVar3) {
    piVar7 = param_5 + 1;
    iVar6 = 1;
    do {
      while (iVar4 = *piVar7, iVar4 - 1U < 6) {
        iVar6 = iVar6 + 1;
        aiStack_58[iVar4] = aiStack_58[iVar4] + 1;
        piVar7 = piVar7 + 1;
        if (iVar3 < iVar6) goto LAB_00a016b8;
      }
      iVar6 = iVar6 + 1;
      piVar7 = piVar7 + 1;
      nrerror_vsdlss(1,"mldorder.c",0xd09);
      iVar3 = *(int *)(param_1 + 8);
      aiStack_58[iVar4] = aiStack_58[iVar4] + 1;
    } while (iVar6 <= iVar3);
  }
LAB_00a016b8:
  uVar2 = (local_44 + local_48) - aiStack_58[3];
  uVar8 = (int)uVar2 >> 0x1f;
  iVar4 = (uVar2 ^ uVar8) - uVar8;
  iVar6 = (local_48 - aiStack_58[2]) - aiStack_58[3];
  if (iVar6 < 1) {
    iVar6 = aiStack_58[3] + (aiStack_58[2] - local_48);
  }
  if (iVar3 < 1) {
    iVar3 = 0;
  }
  else {
    iVar5 = 1;
    iVar3 = 0;
    do {
      param_5 = param_5 + 1;
      iVar1 = *param_5;
      if (((iVar1 - 3U < 2) || ((iVar1 == 5 && (iVar4 < iVar6)))) ||
         ((iVar1 == 2 && (iVar6 <= iVar4)))) {
        iVar3 = iVar3 + 1;
        *(int *)(param_4 + (long)iVar3 * 4) = iVar5;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 <= *(int *)(param_1 + 8));
  }
  return iVar3;
}


