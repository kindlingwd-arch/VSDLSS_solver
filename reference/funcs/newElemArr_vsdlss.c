/* VSDLSS function (decompiled by Ghidra) */
/* name: newElemArr_vsdlss  addr: 009e64e0  size: 8c */
#include "vsdlss_ref.h"

/* signature: int * newElemArr_vsdlss(int param_1,int param_2); */

int * newElemArr_vsdlss(int param_1,int param_2)

{
  int *piVar1;
  undefined8 uVar2;
  long lVar3;
  
  lVar3 = (long)param_1;
  piVar1 = (int *)casiMalloc_vsdlss(0x40);
  *piVar1 = param_1;
  piVar1[1] = param_2;
  *(undefined1 *)(piVar1 + 2) = 0;
  piVar1[3] = 0;
  *(undefined1 *)(piVar1 + 4) = 0;
  uVar2 = newIVector_vsdlss(lVar3,1,0);
  *(undefined8 *)(piVar1 + 6) = uVar2;
  piVar1[8] = 0;
  piVar1[9] = 0;
  uVar2 = newIVector_vsdlss(lVar3,1,0);
  *(undefined8 *)(piVar1 + 10) = uVar2;
  uVar2 = newFVector_vsdlss(0,lVar3,1);
  *(undefined8 *)(piVar1 + 0xc) = uVar2;
  uVar2 = newFVector_vsdlss(0,(long)param_2,1);
  *(undefined8 *)(piVar1 + 0xe) = uVar2;
  return piVar1;
}


