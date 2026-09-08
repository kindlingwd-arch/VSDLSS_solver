/* driver: PRz1pLTaLKQOFzdOLJpLTpyKDBXSPAIPP addr=00982020 size=292 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz1pLTaLKQOFzdOLJpLTpyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  double dVar13;
  double dVar14;
  
  lVar2 = *(long *)(param_4 + 0x50);
  lVar3 = *(long *)(param_4 + 0x48);
  lVar10 = *(long *)(lVar2 + param_1 * 8);
  lVar9 = *(long *)(lVar3 + param_1 * 8);
  lVar12 = *(long *)(*(long *)(param_4 + 0x58) + param_1 * 8);
  if (*(long *)(lVar10 + lVar9 * 8) < param_2) {
    return;
  }
  dVar14 = *(double *)(lVar12 + 8);
  if (dVar14 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x84,param_1);
    dVar14 = *(double *)(lVar12 + 8);
  }
  uVar4 = DAT_00abd200;
  if (lVar9 < 2) {
    return;
  }
  while (lVar6 = *(long *)(lVar10 + 0x10), lVar6 < param_2) {
    lVar9 = lVar9 + -1;
    lVar12 = lVar12 + 8;
    lVar10 = lVar10 + 8;
    if (lVar9 == 1) {
      return;
    }
  }
  if (param_3 < lVar6) {
    return;
  }
  dVar14 = _DAT_00ab67f8 / dVar14;
  do {
    lVar9 = lVar9 + -1;
    lVar7 = lVar12 + 8;
    dVar13 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar4) * dVar14;
    if ((*(long *)(param_4 + 0x80) == 0) || (lVar6 <= *(long *)(param_4 + 0x88))) {
      lVar12 = *(long *)(*(long *)(param_4 + 0x58) + lVar6 * 8);
      lVar8 = *(long *)(lVar3 + lVar6 * 8);
      if (lVar9 != lVar8) goto LAB_00982120;
LAB_009821ef:
      lVar6 = 1;
      do {
        *(double *)(lVar12 + lVar6 * 8) =
             *(double *)(lVar7 + lVar6 * 8) * dVar13 + *(double *)(lVar12 + lVar6 * 8);
        lVar6 = lVar6 + 1;
      } while (lVar6 <= lVar9);
    }
    else {
      lVar12 = *(long *)(*(long *)(param_4 + 0x80) + (lVar6 - *(long *)(param_4 + 0x88)) * 8);
      lVar8 = *(long *)(lVar3 + lVar6 * 8);
      if (lVar9 == lVar8) goto LAB_009821ef;
LAB_00982120:
      if (lVar9 < lVar8) {
        lVar6 = *(long *)(lVar2 + lVar6 * 8);
        if (0 < lVar8) {
          lVar5 = 1;
          lVar11 = 1;
          do {
            if (*(long *)(lVar10 + 8 + lVar11 * 8) == *(long *)(lVar6 + lVar5 * 8)) {
              lVar1 = lVar11 * 8;
              lVar11 = lVar11 + 1;
              *(double *)(lVar12 + lVar5 * 8) =
                   *(double *)(lVar7 + lVar1) * dVar13 + *(double *)(lVar12 + lVar5 * 8);
            }
          } while ((lVar11 <= lVar9) && (lVar5 = lVar5 + 1, lVar5 <= lVar8));
          if (lVar9 < lVar11) goto LAB_00982191;
        }
        KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0xad);
      }
      else {
        KOBOOLOXSPAIPP(1,"factorapar_ls.c",0xb2);
      }
    }
LAB_00982191:
    if ((lVar9 == 1) ||
       (lVar6 = *(long *)(lVar10 + 0x18), lVar12 = lVar7, lVar10 = lVar10 + 8, param_3 < lVar6)) {
      return;
    }
  } while( true );
}


