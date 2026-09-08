/* VSDLSS function (decompiled by Ghidra) */
/* name: diffTimes_vsdlss  addr: 009d79c0  size: 31 */
#include "vsdlss_ref.h"

/* signature: void diffTimes_vsdlss(long *param_1,long *param_2,ulong *param_3); */

void diffTimes_vsdlss(long *param_1,long *param_2,ulong *param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = *param_2 - *param_1 >> 0x3f;
  *param_3 = *param_2 - *param_1 ^ uVar3;
  lVar1 = param_2[1];
  lVar2 = param_1[1];
  *param_3 = *param_3 - uVar3;
  uVar3 = lVar1 - lVar2 >> 0x3f;
  param_3[1] = lVar1 - lVar2 ^ uVar3;
  param_3[1] = param_3[1] - uVar3;
  return;
}


