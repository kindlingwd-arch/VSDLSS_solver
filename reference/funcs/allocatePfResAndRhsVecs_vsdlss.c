/* VSDLSS function (decompiled by Ghidra) */
/* name: allocatePfResAndRhsVecs_vsdlss  addr: 00a10220  size: 70 */
#include "vsdlss_ref.h"

/* signature: void allocatePfResAndRhsVecs_vsdlss(void); */

void allocatePfResAndRhsVecs_vsdlss(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = *DAT_00ec5ea8;
  iVar2 = DAT_00ec5ea8[1];
  iVar3 = DAT_00ec5ea8[2];
  iVar4 = *(int *)(DAT_00ec5ea0 + 4);
  DAT_00ec5e98 = newVector_vsdlss(0,(long)(iVar2 + iVar3),0);
  DAT_00ec5e90 = newVector_vsdlss(0,(long)iVar4,0);
  DAT_00ec5e88 = newVector_vsdlss(0,(long)(iVar1 - (iVar2 + iVar3 + iVar4)),0);
  DAT_00ec5e80 = DAT_00ec5e88;
  return;
}


