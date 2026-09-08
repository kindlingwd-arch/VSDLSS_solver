/* driver: OBJdOLJZR_HBQkyOHgKZMNXSPAIPP addr=009b2280 size=11b */
#include "vsdlss_ref.h"


void OBJdOLJZR_HBQkyOHgKZMNXSPAIPP(long param_1,ulong *param_2)

{
  ulong *puVar1;
  ulong *puVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  
  uVar3 = param_2[7];
  uVar4 = *param_2;
  puVar2 = (ulong *)(uVar3 + param_1 * 8);
  uVar5 = *puVar2;
  if ((0 < (long)uVar5) && ((long)uVar5 <= (long)(uVar4 * 2))) {
    uVar6 = param_2[4];
    puVar1 = (ulong *)(param_2[8] + param_1 * 8);
    uVar7 = *puVar1;
    if ((long)uVar4 < (long)uVar5) {
      *(ulong *)(param_2[6] + (uVar5 - uVar4) * 8) = uVar7;
    }
    else {
      *(ulong *)(param_2[8] + uVar5 * 8) = uVar7;
    }
    if (uVar7 != uVar6) {
      if (((long)uVar4 < (long)uVar7) || ((long)uVar7 < 1)) {
        KOBOOLOXSPAIPP(1,"bpqutils.c",0xa8);
      }
      *(ulong *)(uVar3 + uVar7 * 8) = uVar5;
    }
    param_2[2] = param_2[2] - 1;
    *puVar1 = param_2[5];
    *puVar2 = ~uVar4;
    param_2[5] = -param_1;
    uVar5 = *puVar1;
    if (uVar5 != uVar6) {
      if ((-uVar4 != uVar5 && (long)uVar4 <= (long)-uVar5) || ((long)-uVar5 < 1)) {
        KOBOOLOXSPAIPP(1,"bpqutils.c",0xb7);
      }
      *(long *)(uVar3 + uVar5 * -8) = -param_1;
    }
    param_2[3] = param_2[3] + 1;
  }
  return;
}


