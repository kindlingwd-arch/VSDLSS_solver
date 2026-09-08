/* VSDLSS function (decompiled by Ghidra) */
/* name: newAht_vsdlss  addr: 00a2f470  size: ea */
#include "vsdlss_ref.h"

/* signature: int * newAht_vsdlss(int param_1); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * newAht_vsdlss(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  double dVar8;
  
  iVar6 = 2;
  if (1 < param_1) {
    iVar6 = param_1;
  }
  if (iVar6 < 0x10) {
    iVar4 = 1;
  }
  else {
    dVar8 = log((double)iVar6 * DAT_00aba108);
    dVar8 = floor(dVar8 / _DAT_00b1ddf8);
    iVar4 = (int)dVar8;
    if (iVar4 < 1) {
      lVar7 = 1;
      iVar4 = 0;
      iVar5 = 1;
      goto LAB_00a2f4b0;
    }
  }
  iVar1 = 1;
  iVar5 = 1;
  do {
    iVar1 = iVar1 + 1;
    iVar5 = iVar5 * 2;
  } while (iVar1 <= iVar4);
  iVar4 = iVar5 + -1;
  lVar7 = (long)iVar5;
LAB_00a2f4b0:
  piVar2 = (int *)casiMalloc_vsdlss(0x30);
  *piVar2 = iVar6;
  piVar2[1] = iVar5;
  piVar2[2] = iVar4;
  uVar3 = newIVector_vsdlss(lVar7,1,0);
  *(undefined8 *)(piVar2 + 4) = uVar3;
  uVar3 = newIPVector_vsdlss(lVar7,1,0);
  *(undefined8 *)(piVar2 + 6) = uVar3;
  uVar3 = newIVector_vsdlss((long)iVar6,0,0);
  *(undefined8 *)(piVar2 + 8) = uVar3;
  uVar3 = newIVector_vsdlss((long)iVar6,0,0);
  *(undefined8 *)(piVar2 + 10) = uVar3;
  return piVar2;
}


