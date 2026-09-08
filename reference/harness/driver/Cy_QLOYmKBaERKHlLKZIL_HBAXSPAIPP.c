/* driver: Cy_QLOYmKBaERKHlLKZIL_HBAXSPAIPP addr=0096cc50 size=277 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void Cy_QLOYmKBaERKHlLKZIL_HBAXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
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
  double dVar17;
  double dVar18;
  long local_48;
  
  uVar10 = DAT_00abd200;
  lVar2 = *(long *)(param_1 + 0x10);
  lVar3 = *(long *)(param_1 + 0x50);
  lVar4 = *(long *)(param_1 + 0x58);
  lVar5 = *(long *)(param_1 + 0x48);
  local_48 = 1;
  if (0 < lVar2) {
    do {
      lVar12 = *(long *)(lVar4 + local_48 * 8);
      dVar18 = *(double *)(lVar12 + 8);
      if (dVar18 <= 0.0) {
        KOBOOLOXSPAIPP(dVar18,0x23,"vsdlss1memutils.c",0x883,local_48);
        lVar12 = *(long *)(lVar4 + local_48 * 8);
      }
      lVar6 = *(long *)(lVar5 + local_48 * 8);
      dVar18 = _DAT_00ab67f8 / dVar18;
      if (1 < lVar6) {
        lVar15 = 2;
        lVar9 = *(long *)(lVar3 + local_48 * 8);
        lVar13 = lVar6;
        do {
          lVar14 = lVar13 + -1;
          lVar16 = lVar12 + 8;
          lVar11 = *(long *)(lVar9 + 0x10);
          lVar7 = *(long *)(lVar5 + lVar11 * 8);
          lVar8 = *(long *)(lVar4 + lVar11 * 8);
          dVar17 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar10) * dVar18;
          if (lVar14 == lVar7) {
            lVar12 = 1;
            if (0 < lVar14) {
              do {
                *(double *)(lVar8 + lVar12 * 8) =
                     *(double *)(lVar16 + lVar12 * 8) * dVar17 + *(double *)(lVar8 + lVar12 * 8);
                lVar12 = lVar12 + 1;
              } while (lVar12 != lVar13);
            }
          }
          else if (lVar14 < lVar7) {
            lVar12 = *(long *)(lVar3 + lVar11 * 8);
            if ((lVar7 < 1) || (lVar14 < 1)) {
              lVar13 = 1;
            }
            else {
              lVar11 = 1;
              lVar13 = 1;
              do {
                if (*(long *)(lVar9 + 8 + lVar13 * 8) == *(long *)(lVar12 + lVar11 * 8)) {
                  lVar1 = lVar13 * 8;
                  lVar13 = lVar13 + 1;
                  *(double *)(lVar8 + lVar11 * 8) =
                       *(double *)(lVar16 + lVar1) * dVar17 + *(double *)(lVar8 + lVar11 * 8);
                }
              } while ((lVar13 <= lVar14) && (lVar11 = lVar11 + 1, lVar11 <= lVar7));
            }
            if (lVar13 <= lVar14) {
              KOBOOLOXSPAIPP(0x25,"vsdlss1memutils.c",0x8a5);
            }
          }
          else {
            KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x8aa);
          }
          lVar15 = lVar15 + 1;
          lVar9 = lVar9 + 8;
          lVar12 = lVar16;
          lVar13 = lVar14;
        } while (lVar15 <= lVar6);
      }
      local_48 = local_48 + 1;
    } while (local_48 <= lVar2);
  }
  return;
}


