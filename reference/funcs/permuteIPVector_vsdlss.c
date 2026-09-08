/* VSDLSS function (decompiled by Ghidra) */
/* name: permuteIPVector_vsdlss  addr: 009d8f30  size: 27 */
#include "vsdlss_ref.h"

/* signature: void permuteIPVector_vsdlss(long param_1,long param_2,int param_3,long param_4); */

void permuteIPVector_vsdlss(long param_1,long param_2,int param_3,long param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if ((long)param_3 != 0) {
    do {
      lVar1 = uVar3 * 4;
      lVar2 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      *(undefined8 *)(param_4 + (long)*(int *)(param_1 + lVar1) * 8) =
           *(undefined8 *)(param_2 + lVar2);
    } while (uVar3 <= (ulong)(long)param_3);
  }
  return;
}


