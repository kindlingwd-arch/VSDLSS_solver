/* VSDLSS function (decompiled by Ghidra) */
/* name: permuteDVector_vsdlss  addr: 009d9190  size: 2a */
#include "vsdlss_ref.h"

/* signature: void permuteDVector_vsdlss(long param_1,int param_2,long param_3,long param_4); */

void permuteDVector_vsdlss(long param_1,int param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if ((long)param_2 != 0) {
    do {
      lVar1 = uVar3 * 4;
      lVar2 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      *(undefined8 *)(param_4 + (long)*(int *)(param_1 + lVar1) * 8) =
           *(undefined8 *)(param_3 + lVar2);
    } while (uVar3 <= (ulong)(long)param_2);
  }
  return;
}


