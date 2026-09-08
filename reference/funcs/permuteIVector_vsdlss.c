/* VSDLSS function (decompiled by Ghidra) */
/* name: permuteIVector_vsdlss  addr: 009d8f00  size: 27 */
#include "vsdlss_ref.h"

/* signature: void permuteIVector_vsdlss(long param_1,long param_2,int param_3,long param_4); */

void permuteIVector_vsdlss(long param_1,long param_2,int param_3,long param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if ((long)param_3 != 0) {
    do {
      lVar1 = uVar3 * 4;
      lVar2 = uVar3 * 4;
      uVar3 = uVar3 + 1;
      *(undefined4 *)(param_4 + (long)*(int *)(param_1 + lVar1) * 4) =
           *(undefined4 *)(param_2 + lVar2);
    } while (uVar3 <= (ulong)(long)param_3);
  }
  return;
}


