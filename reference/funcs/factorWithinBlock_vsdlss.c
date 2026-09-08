/* VSDLSS function (decompiled by Ghidra) */
/* name: factorWithinBlock_vsdlss  addr: 009eeb60  size: 4f3 */
#include "vsdlss_ref.h"

/* signature: void factorWithinBlock_vsdlss(int param_1,int param_2,long param_3); */

void factorWithinBlock_vsdlss(int param_1,int param_2,long param_3)

{
  long lVar1;
  long lVar2;
  double dVar3;
  ulong uVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  long *plVar8;
  ulong uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  int iVar17;
  undefined8 *puVar18;
  long lVar19;
  uint uVar20;
  long *plVar21;
  double dVar22;
  int local_214;
  long local_1e0;
  long local_1d8;
  undefined1 local_1b8 [8];
  long local_1b0;
  long local_1a8;
  undefined8 local_1a0;
  undefined1 local_198 [8];
  long local_190 [8];
  double local_150 [7];
  undefined1 local_118 [64];
  double local_d8 [7];
  double local_a0 [7];
  double local_68 [7];
  
  uVar9 = DAT_00abd200;
  param_2 = param_2 - param_1;
  iVar10 = param_2 + 1;
  iVar5 = *(int *)(*(long *)(param_3 + 0x28) + (long)param_1 * 4);
  lVar1 = (long)param_1 * 8 + -8;
  lVar13 = lVar1 + *(long *)(param_3 + 0x38);
  lVar6 = *(long *)(param_3 + 0x30);
  if (0 < iVar10) {
    local_1d8 = -8 - lVar13;
    plVar21 = (long *)(lVar13 + 8);
    local_1e0 = 1;
    do {
      local_214 = 6;
      iVar12 = (int)local_1e0;
      if (iVar10 - iVar12 < 6) {
        local_214 = (param_2 + 2) - iVar12;
      }
      iVar17 = local_214 + iVar12;
      if (iVar12 < iVar17) {
        uVar20 = (iVar17 - iVar12) - 2;
        lVar14 = local_1e0;
        plVar8 = plVar21;
        while( true ) {
          lVar15 = *plVar8;
          dVar3 = *(double *)(lVar15 + 8);
          local_150[0] = dVar3;
          if (dVar3 <= 0.0) {
            nrerror_vsdlss(dVar3,0x23,"factora.c",0xcfe,
                           *(undefined4 *)(*(long *)(lVar1 + lVar6 + lVar14 * 8) + 4));
          }
          if (uVar20 == 0xffffffff) break;
          lVar14 = lVar14 + 1;
          lVar15 = lVar15 + 8;
          lVar2 = (ulong)uVar20 * 8 + 8;
          uVar11 = ((iVar5 + 2) - iVar17) + uVar20;
          lVar19 = 0;
          do {
            while( true ) {
              uVar4 = *(ulong *)(lVar15 + 8);
              lVar7 = *(long *)((long)(plVar8 + 1) + lVar19);
              if ((int)uVar11 < 1) break;
              uVar11 = uVar11 - 1;
              lVar16 = 0;
              do {
                *(double *)(lVar7 + 8 + lVar16) =
                     *(double *)(lVar15 + 8 + lVar16) * ((double)(uVar4 ^ uVar9) / dVar3) +
                     *(double *)(lVar7 + 8 + lVar16);
                lVar16 = lVar16 + 8;
              } while (lVar16 != (ulong)uVar11 * 8 + 8);
              lVar19 = lVar19 + 8;
              lVar15 = lVar15 + 8;
              if (lVar19 == lVar2) goto LAB_009eecdc;
            }
            lVar19 = lVar19 + 8;
            uVar11 = uVar11 - 1;
            lVar15 = lVar15 + 8;
          } while (lVar19 != lVar2);
LAB_009eecdc:
          uVar20 = uVar20 - 1;
          plVar8 = plVar8 + 1;
        }
      }
      if (0 < local_214) {
        lVar14 = 0;
        do {
          *(undefined8 *)((long)local_150 + lVar14) =
               *(undefined8 *)(*(long *)((long)plVar21 + lVar14) + 8);
          lVar14 = lVar14 + 8;
        } while (lVar14 != (ulong)(local_214 - 1) * 8 + 8);
      }
      if (iVar17 <= iVar10) {
        iVar12 = iVar5 - iVar17;
        puVar18 = (undefined8 *)(lVar13 + 0x10 + (long)iVar17 * 8);
        do {
          local_1b0 = puVar18[-2];
          if (iVar17 < iVar10) {
            local_1a8 = puVar18[-1];
          }
          if (0 < local_214) {
            lVar14 = 0;
            do {
              dVar3 = *(double *)((long)local_150 + lVar14);
              plVar8 = (long *)((long)local_190 + lVar14);
              lVar15 = (long)puVar18 +
                       *(long *)((long)plVar21 + lVar14) + ((local_1d8 + -0x10) - lVar14);
              uVar4 = *(ulong *)(lVar15 + 8);
              *plVar8 = lVar15;
              dVar22 = (double)(uVar4 ^ uVar9) / dVar3;
              *(double *)((long)local_d8 + lVar14) = dVar22;
              if (iVar17 < iVar10) {
                *(double *)(local_1b0 + 8) =
                     dVar22 * *(double *)(lVar15 + 8) + *(double *)(local_1b0 + 8);
                *plVar8 = lVar15 + 8;
                *(double *)((long)local_a0 + lVar14) =
                     (double)(*(ulong *)(lVar15 + 0x10) ^ uVar9) / dVar3;
                if (iVar17 < param_2) {
                  *(double *)(local_1b0 + 0x10) =
                       *(double *)((long)local_d8 + lVar14) * *(double *)(lVar15 + 0x10) +
                       *(double *)(local_1b0 + 0x10);
                  *(double *)(local_1a8 + 8) =
                       *(double *)((long)local_a0 + lVar14) * *(double *)(lVar15 + 0x10) +
                       *(double *)(local_1a8 + 8);
                  *plVar8 = lVar15 + 0x10;
                  *(double *)((long)local_68 + lVar14) =
                       (double)(*(ulong *)(lVar15 + 0x18) ^ uVar9) / dVar3;
                }
              }
              lVar14 = lVar14 + 8;
            } while (lVar14 != (ulong)(local_214 - 1) * 8 + 8);
          }
          if (iVar17 < iVar10) {
            if (iVar17 < param_2) {
              local_1b0 = local_1b0 + 0x10;
              local_1a8 = local_1a8 + 8;
              local_1a0 = *puVar18;
              multipleSaxpy3by6_vsdlss(local_214,iVar12 + -1,local_118,local_198,local_1b8);
            }
            else {
              local_1b0 = local_1b0 + 8;
              multipleSaxpy2by6_vsdlss(local_214,iVar12,local_118,local_198,local_1b8);
            }
          }
          else {
            multipleSaxpy1by6_vsdlss(local_214,iVar12 + 1,local_118,local_198,local_1b8);
          }
          iVar17 = iVar17 + 3;
          iVar12 = iVar12 + -3;
          puVar18 = puVar18 + 3;
        } while (iVar17 <= iVar10);
      }
      local_1e0 = local_1e0 + 6;
      plVar21 = plVar21 + 6;
      local_1d8 = local_1d8 + -0x30;
    } while ((int)local_1e0 <= iVar10);
  }
  return;
}


