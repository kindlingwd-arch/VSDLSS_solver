/* VSDLSS function (decompiled by Ghidra) */
/* name: newAffectedByChunk_vsdlss  addr: 00a1be50  size: 9f */
#include "vsdlss_ref.h"

/* signature: int * newAffectedByChunk_vsdlss(int param_1); */

int * newAffectedByChunk_vsdlss(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  
  piVar2 = (int *)casiMalloc_vsdlss(0x18);
  *piVar2 = param_1;
  uVar3 = newCVector_vsdlss((long)(((param_1 + -1) * param_1) / 2),1,0);
  *(undefined8 *)(piVar2 + 2) = uVar3;
  lVar4 = newCPVector_vsdlss((long)*piVar2,1,0);
  *(long *)(piVar2 + 4) = lVar4;
  if (1 < param_1) {
    *(undefined8 *)(lVar4 + 0x10) = *(undefined8 *)(piVar2 + 2);
    iVar1 = *piVar2;
    if (2 < iVar1) {
      lVar5 = 0x18;
      lVar4 = 1;
      do {
        *(long *)(*(long *)(piVar2 + 4) + lVar5) =
             lVar4 + *(long *)(*(long *)(piVar2 + 4) + -8 + lVar5);
        lVar5 = lVar5 + 8;
        lVar4 = lVar4 + 1;
      } while (lVar5 != (ulong)(iVar1 - 3) * 8 + 0x20);
    }
  }
  return piVar2;
}


