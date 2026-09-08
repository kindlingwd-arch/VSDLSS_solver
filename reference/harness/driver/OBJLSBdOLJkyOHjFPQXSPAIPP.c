/* driver: OBJLSBdOLJkyOHjFPQXSPAIPP addr=009b23b0 size=e9 */
#include "vsdlss_ref.h"


long OBJLSBdOLJkyOHjFPQXSPAIPP(ulong *param_1)

{
  ulong *puVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  ulong uVar7;
  
  uVar5 = param_1[5];
  uVar7 = param_1[4];
  if (uVar5 == uVar7) {
    KOBOOLOXSPAIPP(1,"bpqutils.c",0xd2);
    uVar7 = param_1[4];
    uVar5 = param_1[5];
  }
  uVar2 = *param_1;
  lVar6 = -uVar5;
  uVar3 = param_1[7];
  uVar4 = param_1[8];
  if ((-uVar2 != uVar5 && (long)uVar2 <= lVar6) || (lVar6 < 1)) {
    KOBOOLOXSPAIPP(1,"bpqutils.c",0xdd);
  }
  puVar1 = (ulong *)(uVar4 + uVar5 * -8);
  uVar4 = *puVar1;
  param_1[5] = uVar4;
  if (uVar4 != uVar7) {
    if ((-uVar2 != uVar4 && (long)uVar2 <= (long)-uVar4) || ((long)-uVar4 < 1)) {
      KOBOOLOXSPAIPP(1,"bpqutils.c",0xe5);
    }
    *(ulong *)(uVar3 + uVar4 * -8) = ~uVar2;
  }
  param_1[3] = param_1[3] - 1;
  *(ulong *)(uVar3 + uVar5 * -8) = uVar7;
  *puVar1 = uVar7;
  return lVar6;
}


