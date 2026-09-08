/* VSDLSS function (decompiled by Ghidra) */
/* name: newListMark_vsdlss  addr: 009f6a60  size: 44 */
#include "vsdlss_ref.h"

/* signature: int * newListMark_vsdlss(int param_1); */

int * newListMark_vsdlss(int param_1)

{
  int *piVar1;
  undefined8 uVar2;
  
  piVar1 = (int *)casiMalloc_vsdlss(0x20);
  *piVar1 = param_1;
  uVar2 = newCVector_vsdlss((long)param_1,1,0);
  piVar1[4] = 0;
  piVar1[5] = 0;
  *(undefined8 *)(piVar1 + 2) = uVar2;
  piVar1[6] = 0;
  piVar1[7] = 0;
  return piVar1;
}


