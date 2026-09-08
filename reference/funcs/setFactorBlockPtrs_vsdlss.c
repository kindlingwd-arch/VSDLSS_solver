/* VSDLSS function (decompiled by Ghidra) */
/* name: setFactorBlockPtrs_vsdlss  addr: 009f33d0  size: 8e */
#include "vsdlss_ref.h"

/* signature: void setFactorBlockPtrs_vsdlss(int param_1,long param_2,long param_3,int *param_4); */

void setFactorBlockPtrs_vsdlss(int param_1,long param_2,long param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  uint uVar10;
  
  lVar8 = (long)param_1;
  lVar3 = *(long *)(param_3 + 0x30);
  lVar4 = *(long *)(param_3 + 0x38);
  lVar5 = *(long *)(param_4 + 0xc);
  lVar6 = *(long *)(param_4 + 10);
  iVar1 = *(int *)(*(long *)(param_2 + 8) + lVar8 * 4);
  iVar2 = *(int *)(param_3 + 0xc);
  iVar9 = *(int *)(*(long *)(param_2 + 0x10) + lVar8 * 4) + iVar1 + -1;
  lVar7 = *(long *)(param_2 + 0x18);
  param_4[1] = iVar1;
  param_4[2] = iVar9;
  uVar10 = iVar9 - iVar1;
  iVar9 = uVar10 + 1;
  *param_4 = iVar9;
  param_4[3] = *(int *)(lVar7 + lVar8 * 4);
  if (0 < iVar9) {
    lVar7 = 0;
    lVar8 = (long)((iVar1 - iVar2) + 1) * 8;
    do {
      *(undefined8 *)(lVar6 + 8 + lVar7) = *(undefined8 *)(lVar3 + lVar8 + lVar7);
      *(undefined8 *)(lVar5 + 8 + lVar7) = *(undefined8 *)(lVar8 + lVar4 + lVar7);
      lVar7 = lVar7 + 8;
    } while (lVar7 != (ulong)uVar10 * 8 + 8);
  }
  return;
}


