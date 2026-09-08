/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDTagNewAhtUseVecs_vsdlss  addr: 00a27ee0  size: 11d */
#include "vsdlss_ref.h"

/* signature: int * MLDTagNewAhtUseVecs_vsdlss(ulong param_1,int param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,undefined8 param_6); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * MLDTagNewAhtUseVecs_vsdlss
                (ulong param_1,int param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,
                undefined8 param_6)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  int iVar6;
  double dVar7;
  
  iVar6 = 2;
  if (1 < param_2) {
    iVar6 = param_2;
  }
  if (iVar6 < 0x10) {
    iVar4 = 1;
  }
  else {
    dVar7 = log((double)iVar6 * DAT_00aba108);
    dVar7 = floor(dVar7 / _DAT_00b1ddf8);
    iVar4 = (int)dVar7;
    param_1 = param_1 & 0xffffffff;
    if (iVar4 < 1) {
      lVar5 = 1;
      iVar4 = 0;
      iVar3 = 1;
      goto LAB_00a27f33;
    }
  }
  iVar1 = 1;
  iVar3 = 1;
  do {
    iVar1 = iVar1 + 1;
    iVar3 = iVar3 * 2;
  } while (iVar1 <= iVar4);
  iVar4 = iVar3 + -1;
  lVar5 = (long)iVar3;
LAB_00a27f33:
  piVar2 = (int *)newBlock_vsdlss(param_1,0x30);
  *piVar2 = iVar6;
  piVar2[1] = iVar3;
  *(undefined8 *)(piVar2 + 4) = param_3;
  piVar2[2] = iVar4;
  initIVector_vsdlss(param_3,lVar5,0);
  *(undefined8 *)(piVar2 + 6) = param_4;
  initIPVector_vsdlss(param_4,lVar5,0);
  *(undefined8 *)(piVar2 + 8) = param_5;
  *(undefined8 *)(piVar2 + 10) = param_6;
  return piVar2;
}


