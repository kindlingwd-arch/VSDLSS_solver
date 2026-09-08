/* driver: PRz1pLTaLKQOFzdOLJgKApyKDBXSPAIPP addr=009822e0 size=26a */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz1pLTaLKQOFzdOLJgKApyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  double dVar13;
  double dVar14;
  
  uVar5 = DAT_00abd200;
  lVar3 = *(long *)(param_4 + 0x48);
  lVar4 = *(long *)(param_4 + 0x50);
  lVar12 = *(long *)(*(long *)(param_4 + 0x58) + param_1 * 8);
  lVar10 = *(long *)(lVar3 + param_1 * 8);
  if (param_2 < 2) {
    param_2 = 2;
  }
  if (lVar10 <= param_3) {
    param_3 = lVar10;
  }
  if (param_3 < param_2) {
    return;
  }
  lVar2 = param_2 * 8 + -0x10;
  dVar14 = _DAT_00ab67f8 / *(double *)(lVar12 + 8);
  lVar8 = *(long *)(lVar4 + param_1 * 8) + lVar2;
  lVar10 = (2 - param_2) + lVar10;
  lVar12 = lVar12 + lVar2;
  do {
    lVar2 = lVar10 + -1;
    lVar11 = *(long *)(lVar8 + 0x10);
    dVar13 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar5) * dVar14;
    if ((*(long *)(param_4 + 0x80) == 0) || (lVar11 <= *(long *)(param_4 + 0x88))) {
      lVar9 = *(long *)(lVar3 + lVar11 * 8);
      lVar7 = *(long *)(*(long *)(param_4 + 0x58) + lVar11 * 8);
      lVar6 = lVar2 - lVar9;
      if (lVar6 == 0) goto LAB_0098246d;
LAB_009823be:
      if (SBORROW8(lVar2,lVar9) == lVar6 < 0) {
        KOBOOLOXSPAIPP(1,"factorapar_ls.c",0xf6);
      }
      else {
        lVar10 = *(long *)(lVar4 + lVar11 * 8);
        if ((lVar9 < 1) || (lVar2 < 1)) {
          lVar11 = 1;
        }
        else {
          lVar6 = 1;
          lVar11 = 1;
          do {
            if (*(long *)(lVar8 + 8 + lVar11 * 8) == *(long *)(lVar10 + lVar6 * 8)) {
              lVar1 = lVar11 * 8;
              lVar11 = lVar11 + 1;
              *(double *)(lVar7 + lVar6 * 8) =
                   *(double *)(lVar12 + 8 + lVar1) * dVar13 + *(double *)(lVar7 + lVar6 * 8);
            }
          } while ((lVar11 <= lVar2) && (lVar6 = lVar6 + 1, lVar6 <= lVar9));
        }
        if (lVar11 <= lVar2) {
          KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0xf1);
        }
      }
    }
    else {
      lVar7 = *(long *)(*(long *)(param_4 + 0x80) + (lVar11 - *(long *)(param_4 + 0x88)) * 8);
      lVar9 = *(long *)(lVar3 + lVar11 * 8);
      lVar6 = lVar2 - lVar9;
      if (lVar2 != lVar9) goto LAB_009823be;
LAB_0098246d:
      lVar11 = 1;
      if (0 < lVar2) {
        do {
          *(double *)(lVar7 + lVar11 * 8) =
               *(double *)(lVar12 + 8 + lVar11 * 8) * dVar13 + *(double *)(lVar7 + lVar11 * 8);
          lVar11 = lVar11 + 1;
        } while (lVar11 != lVar10);
      }
    }
    if (param_3 < param_2 + 1) {
      return;
    }
    param_2 = param_2 + 1;
    lVar8 = lVar8 + 8;
    lVar10 = lVar2;
    lVar12 = lVar12 + 8;
  } while( true );
}


