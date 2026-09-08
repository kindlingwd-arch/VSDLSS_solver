/* driver: _LJMRQBlLABrLrEOByAXSPAIPP addr=009a3080 size=1e8 */
#include "vsdlss_ref.h"


ulong _LJMRQBlLABrLrEOByAXSPAIPP
                (long param_1,long param_2,long param_3,undefined8 param_4,long param_5,long param_6
                ,long param_7,long param_8,long param_9)

{
  ulong *puVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long *plVar11;
  ulong local_248 [67];
  
  lVar10 = param_1 + param_2 + param_3;
  puVar1 = (ulong *)(*(long *)(param_5 + 0x18) + *(long *)(param_5 + 8) * 0x28);
  uVar2 = *puVar1;
  uVar4 = puVar1[4];
  uVar3 = puVar1[3];
  if (uVar2 != 0) {
    uVar5 = 1;
    do {
      local_248[uVar5] = (ulong)(param_3 * 3 + param_1 + param_2 * 2) / uVar2;
      uVar5 = uVar5 + 1;
    } while (uVar5 <= uVar2);
  }
  FKFQgtB_QLOXSPAIPP(param_8,param_4,0);
  lVar9 = lVar10 + uVar3;
  if (0 < lVar9) {
    plVar11 = (long *)(uVar4 + uVar3 * 0x38);
    do {
      if ((long)uVar2 < 2) {
        lVar8 = 1;
      }
      else {
        lVar6 = 2;
        lVar8 = 1;
        uVar4 = local_248[1];
        do {
          if (uVar4 < local_248[lVar6]) {
            uVar4 = local_248[lVar6];
            lVar8 = lVar6;
          }
          lVar6 = lVar6 + 1;
        } while (lVar6 != uVar2 + 1);
      }
      if (lVar10 < lVar9) {
        lVar7 = lVar10 + *plVar11;
        lVar6 = plVar11[1];
        if (lVar7 <= lVar10 + lVar6) {
          uVar4 = local_248[lVar8];
          do {
            *(long *)(param_8 + lVar7 * 8) = lVar8;
            uVar4 = uVar4 - *(long *)(param_7 + lVar7 * 8);
            lVar7 = lVar7 + 1;
          } while (lVar7 != lVar10 + lVar6 + 1);
          local_248[lVar8] = uVar4;
        }
      }
      else if (*(long *)(param_6 + lVar9 * 8) == lVar9) {
        *(long *)(param_8 + lVar9 * 8) = lVar8;
        local_248[lVar8] = local_248[lVar8] - *(long *)(param_7 + lVar9 * 8);
      }
      plVar11 = plVar11 + -7;
      lVar9 = lVar9 + -1;
    } while (lVar9 != 0);
  }
  lVar9 = 1;
  if (0 < lVar10) {
    do {
      while( true ) {
        plVar11 = (long *)(param_8 + *(long *)(param_6 + lVar9 * 8) * 8);
        lVar8 = *plVar11;
        if (lVar8 != 0) break;
        KOBOOLOXSPAIPP(1,"prepostpar.c",0xe6);
        *(long *)(param_9 + lVar9 * 8) = *plVar11;
        lVar9 = lVar9 + 1;
        if (lVar10 < lVar9) {
          return uVar2;
        }
      }
      *(long *)(param_9 + lVar9 * 8) = lVar8;
      lVar9 = lVar9 + 1;
    } while (lVar9 <= lVar10);
  }
  return uVar2;
}


