/* VSDLSS function (decompiled by Ghidra) */
/* name: newBlockInfo_vsdlss  addr: 00a1e9b0  size: 85 */
#include "vsdlss_ref.h"

/* signature: int * newBlockInfo_vsdlss(int param_1,int param_2); */

int * newBlockInfo_vsdlss(int param_1,int param_2)

{
  int *piVar1;
  undefined8 uVar2;
  long lVar3;
  
  lVar3 = (long)param_2;
  piVar1 = (int *)casiMalloc_vsdlss(0x38);
  *piVar1 = param_1;
  piVar1[1] = param_2;
  uVar2 = newIVector_vsdlss(lVar3,0,0);
  *(undefined8 *)(piVar1 + 2) = uVar2;
  uVar2 = newIVector_vsdlss(lVar3,0,0);
  *(undefined8 *)(piVar1 + 4) = uVar2;
  uVar2 = newIVector_vsdlss(lVar3,0,0);
  *(undefined8 *)(piVar1 + 6) = uVar2;
  uVar2 = newLIVector_vsdlss(lVar3,0,0);
  *(undefined8 *)(piVar1 + 8) = uVar2;
  uVar2 = newIVector_vsdlss(lVar3,0,0);
  *(undefined8 *)(piVar1 + 10) = uVar2;
  uVar2 = newIVector_vsdlss((long)param_1,0,0);
  *(undefined8 *)(piVar1 + 0xc) = uVar2;
  return piVar1;
}


