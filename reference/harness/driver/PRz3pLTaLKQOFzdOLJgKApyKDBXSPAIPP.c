/* driver: PRz3pLTaLKQOFzdOLJgKApyKDBXSPAIPP addr=00983240 size=353 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz3pLTaLKQOFzdOLJgKApyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  long local_80;
  
  uVar7 = DAT_00abd200;
  lVar4 = *(long *)(param_4 + 0x50);
  lVar5 = *(long *)(param_4 + 0x48);
  lVar17 = 4;
  if (3 < param_2) {
    lVar17 = param_2;
  }
  lVar13 = *(long *)(param_4 + 0x58);
  lVar16 = *(long *)(lVar13 + param_1 * 8);
  lVar15 = *(long *)(lVar13 + 8 + param_1 * 8);
  lVar13 = *(long *)(lVar13 + 0x10 + param_1 * 8);
  lVar6 = *(long *)(lVar5 + param_1 * 8);
  if (lVar6 <= param_3) {
    param_3 = lVar6;
  }
  if (param_3 < lVar17) {
    return;
  }
  lVar10 = lVar17 * 8;
  dVar21 = _DAT_00ab67f8 / *(double *)(lVar13 + 8);
  dVar22 = _DAT_00ab67f8 / *(double *)(lVar15 + 8);
  dVar23 = _DAT_00ab67f8 / *(double *)(lVar16 + 8);
  lVar11 = lVar10 + -0x10 + *(long *)(lVar4 + param_1 * 8);
  lVar13 = lVar13 + -0x20 + lVar10;
  lVar15 = lVar15 + -0x18 + lVar10;
  lVar16 = lVar16 + lVar10 + -0x10;
  local_80 = (lVar6 - lVar17) + 2;
  do {
    lVar10 = *(long *)(lVar11 + 0x10);
    lVar6 = local_80 + -1;
    dVar18 = (double)(*(ulong *)(lVar16 + 0x10) ^ uVar7) * dVar23;
    dVar19 = (double)(*(ulong *)(lVar15 + 0x10) ^ uVar7) * dVar22;
    dVar20 = (double)(*(ulong *)(lVar13 + 0x10) ^ uVar7) * dVar21;
    if ((*(long *)(param_4 + 0x80) == 0) || (lVar10 <= *(long *)(param_4 + 0x88))) {
      lVar8 = *(long *)(*(long *)(param_4 + 0x58) + lVar10 * 8);
      lVar9 = *(long *)(lVar5 + lVar10 * 8);
      lVar14 = lVar6 - lVar9;
      if (lVar14 != 0) goto LAB_00983388;
LAB_0098345d:
      lVar10 = 1;
      if (0 < lVar6) {
        do {
          *(double *)(lVar8 + lVar10 * 8) =
               *(double *)(lVar16 + 8 + lVar10 * 8) * dVar18 +
               *(double *)(lVar15 + 8 + lVar10 * 8) * dVar19 +
               *(double *)(lVar13 + 8 + lVar10 * 8) * dVar20 + *(double *)(lVar8 + lVar10 * 8);
          lVar10 = lVar10 + 1;
        } while (lVar10 != local_80);
      }
    }
    else {
      lVar8 = *(long *)(*(long *)(param_4 + 0x80) + (lVar10 - *(long *)(param_4 + 0x88)) * 8);
      lVar9 = *(long *)(lVar5 + lVar10 * 8);
      lVar14 = lVar6 - lVar9;
      if (lVar6 == lVar9) goto LAB_0098345d;
LAB_00983388:
      if (SBORROW8(lVar6,lVar9) == lVar14 < 0) {
        KOBOOLOXSPAIPP(1,"factorapar_ls.c",0x27f);
      }
      else {
        lVar10 = *(long *)(lVar4 + lVar10 * 8);
        if ((lVar9 < 1) || (lVar6 < 1)) {
          lVar14 = 1;
        }
        else {
          lVar12 = 1;
          lVar14 = 1;
          do {
            if (*(long *)(lVar11 + 8 + lVar14 * 8) == *(long *)(lVar10 + lVar12 * 8)) {
              lVar1 = lVar14 * 8;
              lVar3 = lVar14 * 8;
              lVar2 = lVar14 * 8;
              lVar14 = lVar14 + 1;
              *(double *)(lVar8 + lVar12 * 8) =
                   *(double *)(lVar16 + 8 + lVar1) * dVar18 +
                   *(double *)(lVar15 + 8 + lVar3) * dVar19 +
                   *(double *)(lVar13 + 8 + lVar2) * dVar20 + *(double *)(lVar8 + lVar12 * 8);
            }
          } while ((lVar14 <= lVar6) && (lVar12 = lVar12 + 1, lVar12 <= lVar9));
        }
        if (lVar14 <= lVar6) {
          KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0x27a);
        }
      }
    }
    if (param_3 < lVar17 + 1) {
      return;
    }
    lVar17 = lVar17 + 1;
    lVar11 = lVar11 + 8;
    lVar13 = lVar13 + 8;
    lVar15 = lVar15 + 8;
    lVar16 = lVar16 + 8;
    local_80 = lVar6;
  } while( true );
}


