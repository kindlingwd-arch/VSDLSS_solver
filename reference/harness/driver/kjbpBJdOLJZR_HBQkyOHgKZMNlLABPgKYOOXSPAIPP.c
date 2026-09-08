/* driver: kjbpBJdOLJZR_HBQkyOHgKZMNlLABPgKYOOXSPAIPP addr=009b5f00 size=cc */
#include "vsdlss_ref.h"


void kjbpBJdOLJZR_HBQkyOHgKZMNlLABPgKYOOXSPAIPP
               (long param_1,long param_2,long param_3,ulong *param_4)

{
  long *plVar1;
  ulong *puVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  long *plVar9;
  ulong *puVar10;
  
  plVar9 = (long *)(param_2 + 8);
  plVar1 = plVar9 + param_1;
  if (0 < param_1) {
    do {
      while( true ) {
        lVar3 = *plVar9;
        if (*(long *)(param_3 + lVar3 * 8) < 1) break;
LAB_009b5f20:
        plVar9 = plVar9 + 1;
        if (plVar9 == plVar1) {
          return;
        }
      }
      uVar4 = param_4[7];
      uVar5 = *param_4;
      uVar6 = param_4[4];
      puVar2 = (ulong *)(uVar4 + lVar3 * 8);
      uVar7 = *puVar2;
      if (((long)uVar7 < 1) || ((long)(uVar5 * 2) < (long)uVar7)) goto LAB_009b5f20;
      puVar10 = (ulong *)(lVar3 * 8 + param_4[8]);
      uVar8 = *puVar10;
      if ((long)uVar5 < (long)uVar7) {
        *(ulong *)(param_4[6] + (uVar7 - uVar5) * 8) = uVar8;
      }
      else {
        *(ulong *)(param_4[8] + uVar7 * 8) = uVar8;
      }
      if (uVar8 != uVar6) {
        *(ulong *)(uVar4 + uVar8 * 8) = uVar7;
      }
      param_4[2] = param_4[2] - 1;
      *puVar10 = param_4[5];
      *puVar2 = ~uVar5;
      param_4[5] = -lVar3;
      if (*puVar10 != uVar6) {
        *(long *)(uVar4 + *puVar10 * -8) = -lVar3;
      }
      plVar9 = plVar9 + 1;
      param_4[3] = param_4[3] + 1;
    } while (plVar9 != plVar1);
  }
  return;
}


