/* VSDLSS function (decompiled by Ghidra) */
/* name: newBpq_vsdlss  addr: 00a1eae0  size: 7f */
#include "vsdlss_ref.h"

/* signature: int * newBpq_vsdlss(int param_1); */

int * newBpq_vsdlss(int param_1)

{
  int *piVar1;
  undefined8 uVar2;
  long lVar3;
  
  lVar3 = (long)param_1;
  piVar1 = (int *)casiMalloc_vsdlss(0x30);
  *piVar1 = param_1;
  piVar1[1] = param_1;
  piVar1[2] = 0;
  piVar1[3] = 0;
  piVar1[4] = 0x7ffffff5;
  piVar1[5] = 0x7ffffff5;
  uVar2 = newIVector_vsdlss(lVar3,1,0x7ffffff5);
  *(undefined8 *)(piVar1 + 6) = uVar2;
  uVar2 = newIVector_vsdlss(lVar3,1,piVar1[4]);
  *(undefined8 *)(piVar1 + 8) = uVar2;
  uVar2 = newIVector_vsdlss(lVar3,1,piVar1[4]);
  *(undefined8 *)(piVar1 + 10) = uVar2;
  return piVar1;
}


