/* VSDLSS function (decompiled by Ghidra) */
/* name: readAndExtendEaFromToVecs_vsdlss  addr: 00a11e90  size: 16c */
#include "vsdlss_ref.h"

/* signature: void readAndExtendEaFromToVecs_vsdlss(int param_1,int param_2,int param_3,int param_4,long param_5,long param_6); */

void readAndExtendEaFromToVecs_vsdlss
               (int param_1,int param_2,int param_3,int param_4,long param_5,long param_6)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  long lVar10;
  long lVar11;
  int iVar12;
  
  iVar12 = param_2 - param_4;
  param_1 = param_1 - param_3;
  iVar1 = *(int *)(param_5 + 0x10);
  lVar3 = *(long *)(param_5 + 0x30);
  lVar7 = newIVector_vsdlss((long)param_2,0,0);
  *(long *)(param_6 + 0x18) = lVar7;
  lVar8 = newIVector_vsdlss((long)param_2,0,0);
  *(long *)(param_6 + 0x28) = lVar8;
  readIVectorFNum_vsdlss(0x1b,iVar12,lVar7);
  readIVectorFNum_vsdlss(0x1c,iVar12,lVar8);
  checkFromAndToNodes_vsdlss(param_1,iVar12,lVar7,lVar8);
  if (iVar1 < 1) {
    iVar6 = 0;
  }
  else {
    iVar9 = param_1 + 1;
    lVar11 = 0;
    iVar6 = 0;
    lVar4 = *(long *)(param_5 + 0x40);
    do {
      iVar2 = *(int *)(lVar3 + 4 + lVar11);
      lVar5 = **(long **)(lVar4 + 8 + lVar11 * 2);
      if (0 < iVar2) {
        lVar10 = 0;
        do {
          *(undefined4 *)(lVar7 + (long)iVar12 * 4 + (long)iVar6 * 4 + 4 + lVar10) =
               *(undefined4 *)(lVar5 + 4 + lVar10);
          *(int *)((long)iVar6 * 4 + lVar8 + (long)iVar12 * 4 + 4 + lVar10) = iVar9;
          lVar10 = lVar10 + 4;
        } while (lVar10 != (ulong)(iVar2 - 1) * 4 + 4);
        iVar6 = iVar6 + iVar2;
      }
      iVar9 = iVar9 + 1;
      lVar11 = lVar11 + 4;
    } while (iVar9 != iVar1 + 1 + param_1);
  }
  if ((iVar6 == param_4) && (iVar1 == param_3)) {
    return;
  }
  nrerror_vsdlss(1,"reduceproblem.c",0x551);
  return;
}


