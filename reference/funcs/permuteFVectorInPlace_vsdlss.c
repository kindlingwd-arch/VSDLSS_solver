/* VSDLSS function (decompiled by Ghidra) */
/* name: permuteFVectorInPlace_vsdlss  addr: 009d9010  size: b6 */
#include "vsdlss_ref.h"

/* signature: void permuteFVectorInPlace_vsdlss(long param_1,int param_2,long param_3); */

void permuteFVectorInPlace_vsdlss(long param_1,int param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  
  uVar5 = (ulong)param_2;
  lVar1 = uVar5 * 8 + 8;
  lVar3 = casiMalloc_vsdlss(lVar1);
  if (lVar3 == 0) {
    nrerror_vsdlss(0xd,"utils.c",0x5fa,lVar1,"polyNewVectorNoInit()");
  }
  uVar4 = 1;
  if (uVar5 != 0) {
    do {
      lVar2 = uVar4 * 4;
      lVar1 = uVar4 * 8;
      uVar4 = uVar4 + 1;
      *(undefined8 *)(lVar3 + (long)*(int *)(param_1 + lVar2) * 8) =
           *(undefined8 *)(param_3 + lVar1);
    } while (uVar4 <= uVar5);
    uVar4 = 1;
    do {
      *(undefined8 *)(param_3 + uVar4 * 8) = *(undefined8 *)(lVar3 + uVar4 * 8);
      uVar4 = uVar4 + 1;
    } while (uVar4 <= uVar5);
  }
  if (lVar3 == 0) {
    return;
  }
  casiFree_vsdlss(lVar3);
  return;
}


