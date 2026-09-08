/* VSDLSS function (decompiled by Ghidra) */
/* name: invPermuteDVectorInPlace_vsdlss  addr: 009d9230  size: b6 */
#include "vsdlss_ref.h"

/* signature: void invPermuteDVectorInPlace_vsdlss(long param_1,int param_2,long param_3); */

void invPermuteDVectorInPlace_vsdlss(long param_1,int param_2,long param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  ulong uVar4;
  
  uVar4 = (ulong)param_2;
  lVar1 = uVar4 * 8 + 8;
  lVar2 = casiMalloc_vsdlss(lVar1);
  if (lVar2 == 0) {
    nrerror_vsdlss(0xd,"utils.c",0x63a,lVar1,"polyNewVectorNoInit()");
  }
  uVar3 = 1;
  if (uVar4 != 0) {
    do {
      *(undefined8 *)(lVar2 + uVar3 * 8) =
           *(undefined8 *)(param_3 + (long)*(int *)(param_1 + uVar3 * 4) * 8);
      uVar3 = uVar3 + 1;
    } while (uVar3 <= uVar4);
    uVar3 = 1;
    do {
      *(undefined8 *)(param_3 + uVar3 * 8) = *(undefined8 *)(lVar2 + uVar3 * 8);
      uVar3 = uVar3 + 1;
    } while (uVar3 <= uVar4);
  }
  if (lVar2 == 0) {
    return;
  }
  casiFree_vsdlss(lVar2);
  return;
}


