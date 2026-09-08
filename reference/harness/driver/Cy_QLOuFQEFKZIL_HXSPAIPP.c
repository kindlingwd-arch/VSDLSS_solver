/* driver: Cy_QLOuFQEFKZIL_HXSPAIPP addr=00981260 size=487 */
#include "vsdlss_ref.h"


void Cy_QLOuFQEFKZIL_HXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  double dVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long *plVar19;
  double dVar20;
  long local_218;
  long local_210;
  long local_1e8;
  long local_1e0;
  undefined1 local_1b8 [8];
  long local_1b0;
  long local_1a8;
  long local_1a0;
  undefined1 local_198 [8];
  long local_190 [7];
  double adStack_158 [8];
  undefined1 local_118 [64];
  double local_d8 [7];
  double local_a0 [7];
  double local_68 [7];
  
  uVar10 = DAT_00abd200;
  param_2 = param_2 - param_1;
  lVar1 = param_2 + 1;
  lVar6 = *(long *)(*(long *)(param_3 + 0x48) + param_1 * 8);
  lVar3 = param_1 * 8 + -8;
  lVar7 = *(long *)(param_3 + 0x50);
  lVar11 = lVar3 + *(long *)(param_3 + 0x58);
  if (0 < lVar1) {
    local_1e8 = -8 - lVar11;
    lVar17 = lVar11 + 8;
    local_1e0 = 1;
    do {
      local_218 = 6;
      if (lVar1 - local_1e0 < 6) {
        local_218 = (param_2 + 2) - local_1e0;
      }
      lVar14 = local_1e0 + local_218;
      if (local_1e0 < lVar14) {
        lVar18 = lVar6 - local_1e0;
        lVar13 = local_1e0;
        while( true ) {
          lVar8 = *(long *)(lVar11 + lVar13 * 8);
          dVar4 = *(double *)(lVar8 + 8);
          adStack_158[1] = dVar4;
          if (dVar4 <= 0.0) {
            KOBOOLOXSPAIPP(dVar4,0x23,"factora.c",0xcfe,
                           *(undefined8 *)(*(long *)(lVar3 + lVar7 + lVar13 * 8) + 8));
          }
          if (lVar13 + 1 == lVar14) break;
          lVar13 = lVar13 + 1;
          lVar15 = lVar13;
          lVar16 = lVar18;
          do {
            uVar5 = *(ulong *)(lVar8 + 0x10);
            lVar9 = *(long *)(lVar11 + lVar15 * 8);
            lVar12 = 1;
            if (0 < lVar16) {
              do {
                *(double *)(lVar9 + lVar12 * 8) =
                     *(double *)(lVar8 + 8 + lVar12 * 8) * ((double)(uVar5 ^ uVar10) / dVar4) +
                     *(double *)(lVar9 + lVar12 * 8);
                lVar12 = lVar12 + 1;
              } while (lVar12 != lVar16 + 1);
            }
            lVar15 = lVar15 + 1;
            lVar16 = lVar16 + -1;
            lVar8 = lVar8 + 8;
          } while (lVar15 != lVar14);
          lVar18 = lVar18 + -1;
        }
      }
      if (0 < local_218) {
        lVar13 = 1;
        do {
          adStack_158[lVar13] = *(double *)(*(long *)(lVar17 + -8 + lVar13 * 8) + 8);
          lVar13 = lVar13 + 1;
        } while (lVar13 != local_218 + 1);
      }
      if (lVar14 <= lVar1) {
        plVar19 = (long *)(lVar11 + lVar14 * 8);
        local_210 = lVar6 - lVar14;
        do {
          local_1b0 = *plVar19;
          if (lVar14 < lVar1) {
            local_1a8 = plVar19[1];
          }
          if (0 < local_218) {
            lVar13 = 0;
            do {
              dVar4 = *(double *)((long)adStack_158 + lVar13 + 8);
              lVar18 = (long)plVar19 + *(long *)(lVar17 + lVar13) + (local_1e8 - lVar13);
              plVar2 = (long *)((long)local_190 + lVar13);
              uVar5 = *(ulong *)(lVar18 + 8);
              *plVar2 = lVar18;
              dVar20 = (double)(uVar5 ^ uVar10) / dVar4;
              *(double *)((long)local_d8 + lVar13) = dVar20;
              if (lVar14 < lVar1) {
                *(double *)(local_1b0 + 8) =
                     dVar20 * *(double *)(lVar18 + 8) + *(double *)(local_1b0 + 8);
                *plVar2 = lVar18 + 8;
                *(double *)((long)local_a0 + lVar13) =
                     (double)(*(ulong *)(lVar18 + 0x10) ^ uVar10) / dVar4;
                if (lVar14 < param_2) {
                  *(double *)(local_1b0 + 0x10) =
                       *(double *)((long)local_d8 + lVar13) * *(double *)(lVar18 + 0x10) +
                       *(double *)(local_1b0 + 0x10);
                  *(double *)(local_1a8 + 8) =
                       *(double *)((long)local_a0 + lVar13) * *(double *)(lVar18 + 0x10) +
                       *(double *)(local_1a8 + 8);
                  *plVar2 = lVar18 + 0x10;
                  *(double *)((long)local_68 + lVar13) =
                       (double)(*(ulong *)(lVar18 + 0x18) ^ uVar10) / dVar4;
                }
              }
              lVar13 = lVar13 + 8;
            } while (lVar13 != local_218 << 3);
          }
          if (lVar14 < lVar1) {
            if (lVar14 < param_2) {
              local_1b0 = local_1b0 + 0x10;
              local_1a8 = local_1a8 + 8;
              local_1a0 = plVar19[2];
              JRIQFMIBqyUMV3zV6XSPAIPP(local_218,local_210 + -1,local_118,local_198,local_1b8);
            }
            else {
              local_1b0 = local_1b0 + 8;
              JRIQFMIBqyUMV2zV6XSPAIPP(local_218,local_210,local_118,local_198,local_1b8);
            }
          }
          else {
            JRIQFMIBqyUMV1zV6XSPAIPP(local_218,local_210 + 1,local_118,local_198,local_1b8);
          }
          lVar14 = lVar14 + 3;
          plVar19 = plVar19 + 3;
          local_210 = local_210 + -3;
        } while (lVar14 <= lVar1);
      }
      local_1e0 = local_1e0 + 6;
      lVar17 = lVar17 + 0x30;
      local_1e8 = local_1e8 + -0x30;
    } while (local_1e0 <= lVar1);
  }
  return;
}


