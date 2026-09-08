/* driver: PRz2pLTaLKQOFzdOLJgKApyKDBXSPAIPP addr=00982990 size=2d8 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz2pLTaLKQOFzdOLJgKApyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  
  uVar6 = DAT_00abd200;
  lVar4 = *(long *)(param_4 + 0x50);
  lVar5 = *(long *)(param_4 + 0x48);
  lVar15 = 3;
  if (2 < param_2) {
    lVar15 = param_2;
  }
  lVar14 = *(long *)(*(long *)(param_4 + 0x58) + param_1 * 8);
  lVar13 = *(long *)(*(long *)(param_4 + 0x58) + 8 + param_1 * 8);
  lVar8 = *(long *)(lVar5 + param_1 * 8);
  if (lVar8 <= param_3) {
    param_3 = lVar8;
  }
  if (param_3 < lVar15) {
    return;
  }
  lVar1 = lVar15 * 8 + -0x10;
  dVar18 = _DAT_00ab67f8 / *(double *)(lVar13 + 8);
  dVar19 = _DAT_00ab67f8 / *(double *)(lVar14 + 8);
  lVar8 = (lVar8 - lVar15) + 2;
  lVar10 = lVar1 + *(long *)(lVar4 + param_1 * 8);
  lVar13 = lVar13 + -0x18 + lVar15 * 8;
  lVar14 = lVar14 + lVar1;
  do {
    lVar1 = lVar8 + -1;
    lVar9 = *(long *)(lVar10 + 0x10);
    dVar16 = (double)(*(ulong *)(lVar14 + 0x10) ^ uVar6) * dVar19;
    dVar17 = (double)(*(ulong *)(lVar13 + 0x10) ^ uVar6) * dVar18;
    if ((*(long *)(param_4 + 0x80) == 0) || (lVar9 <= *(long *)(param_4 + 0x88))) {
      lVar11 = *(long *)(*(long *)(param_4 + 0x58) + lVar9 * 8);
      lVar12 = *(long *)(lVar5 + lVar9 * 8);
      lVar7 = lVar1 - lVar12;
      if (lVar7 != 0) goto LAB_00982a98;
LAB_00982b5d:
      lVar9 = 1;
      if (0 < lVar1) {
        do {
          *(double *)(lVar11 + lVar9 * 8) =
               *(double *)(lVar14 + 8 + lVar9 * 8) * dVar16 +
               *(double *)(lVar13 + 8 + lVar9 * 8) * dVar17 + *(double *)(lVar11 + lVar9 * 8);
          lVar9 = lVar9 + 1;
        } while (lVar9 != lVar8);
      }
    }
    else {
      lVar11 = *(long *)(*(long *)(param_4 + 0x80) + (lVar9 - *(long *)(param_4 + 0x88)) * 8);
      lVar12 = *(long *)(lVar5 + lVar9 * 8);
      lVar7 = lVar1 - lVar12;
      if (lVar1 == lVar12) goto LAB_00982b5d;
LAB_00982a98:
      if (SBORROW8(lVar1,lVar12) == lVar7 < 0) {
        KOBOOLOXSPAIPP(1,"factorapar_ls.c",0x1a9);
      }
      else {
        lVar8 = *(long *)(lVar4 + lVar9 * 8);
        if ((lVar12 < 1) || (lVar1 < 1)) {
          lVar9 = 1;
        }
        else {
          lVar7 = 1;
          lVar9 = 1;
          do {
            if (*(long *)(lVar10 + 8 + lVar9 * 8) == *(long *)(lVar8 + lVar7 * 8)) {
              lVar2 = lVar9 * 8;
              lVar3 = lVar9 * 8;
              lVar9 = lVar9 + 1;
              *(double *)(lVar11 + lVar7 * 8) =
                   *(double *)(lVar14 + 8 + lVar2) * dVar16 +
                   *(double *)(lVar13 + 8 + lVar3) * dVar17 + *(double *)(lVar11 + lVar7 * 8);
            }
          } while ((lVar9 <= lVar1) && (lVar7 = lVar7 + 1, lVar7 <= lVar12));
        }
        if (lVar9 <= lVar1) {
          KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0x1a4);
        }
      }
    }
    if (param_3 < lVar15 + 1) {
      return;
    }
    lVar15 = lVar15 + 1;
    lVar8 = lVar1;
    lVar10 = lVar10 + 8;
    lVar13 = lVar13 + 8;
    lVar14 = lVar14 + 8;
  } while( true );
}


