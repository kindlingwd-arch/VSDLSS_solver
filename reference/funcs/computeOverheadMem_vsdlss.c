/* VSDLSS function (decompiled by Ghidra) */
/* name: computeOverheadMem_vsdlss  addr: 00a22760  size: 5d */
#include "vsdlss_ref.h"

/* signature: double computeOverheadMem_vsdlss(int *param_1,undefined8 param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

double computeOverheadMem_vsdlss(int *param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *param_1;
  iVar2 = countNumberOfBlocks_vsdlss();
  iVar3 = getMaxIVector_vsdlss(param_2,iVar1);
  return (double)iVar2 * DAT_00b1ab20 + (double)iVar1 * DAT_00ad4f20 + _DAT_00b1ce38 + _DAT_00b1ce40
         + (double)iVar3 * DAT_00ad4f20;
}


