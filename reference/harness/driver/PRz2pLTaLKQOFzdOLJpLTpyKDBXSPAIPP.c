/* driver: PRz2pLTaLKQOFzdOLJpLTpyKDBXSPAIPP addr=00982570 size=406 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz2pLTaLKQOFzdOLJpLTpyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  double dVar5;
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
  
  lVar3 = *(long *)(param_4 + 0x50);
  lVar4 = *(long *)(param_4 + 0x48);
  lVar8 = *(long *)(lVar4 + param_1 * 8);
  lVar14 = *(long *)(lVar3 + param_1 * 8);
  lVar12 = *(long *)(*(long *)(param_4 + 0x58) + param_1 * 8);
  lVar11 = *(long *)(*(long *)(param_4 + 0x58) + 8 + param_1 * 8);
  if (param_2 <= *(long *)(lVar14 + lVar8 * 8)) {
    dVar19 = *(double *)(lVar12 + 8);
    if (dVar19 <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x11d,param_1);
      dVar19 = *(double *)(lVar12 + 8);
    }
    dVar5 = _DAT_00ab67f8;
    lVar13 = lVar8 + -1;
    dVar19 = _DAT_00ab67f8 / dVar19;
    if ((param_2 <= param_1 + 1) &&
       (dVar16 = (double)(*(ulong *)(lVar12 + 0x10) ^ DAT_00abd200), 0 < lVar13)) {
      lVar9 = 1;
      do {
        *(double *)(lVar11 + lVar9 * 8) =
             *(double *)(lVar12 + 8 + lVar9 * 8) * dVar16 * dVar19 + *(double *)(lVar11 + lVar9 * 8)
        ;
        lVar9 = lVar9 + 1;
      } while (lVar9 != lVar8);
    }
    dVar16 = *(double *)(lVar11 + 8);
    if (dVar16 <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",300);
      dVar16 = *(double *)(lVar11 + 8);
    }
    uVar6 = DAT_00abd200;
    if (1 < lVar13) {
      lVar8 = *(long *)(lVar14 + 0x18);
      while( true ) {
        lVar12 = lVar12 + 8;
        if (param_2 <= lVar8) break;
        lVar13 = lVar13 + -1;
        lVar11 = lVar11 + 8;
        if (lVar13 == 1) {
          return;
        }
        lVar8 = *(long *)(lVar14 + 0x20);
        lVar14 = lVar14 + 8;
      }
      if (lVar8 <= param_3) {
        lVar14 = lVar14 + 8;
        do {
          lVar15 = lVar12 + 8;
          lVar13 = lVar13 + -1;
          lVar9 = lVar11 + 8;
          dVar17 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar6) * dVar19;
          dVar18 = (double)(*(ulong *)(lVar11 + 0x10) ^ uVar6) * (dVar5 / dVar16);
          if ((*(long *)(param_4 + 0x80) == 0) || (lVar8 <= *(long *)(param_4 + 0x88))) {
            lVar11 = *(long *)(lVar4 + lVar8 * 8);
            lVar12 = *(long *)(*(long *)(param_4 + 0x58) + lVar8 * 8);
            if (lVar13 == lVar11) goto LAB_00982805;
LAB_00982729:
            if (lVar13 < lVar11) {
              lVar8 = *(long *)(lVar3 + lVar8 * 8);
              if (0 < lVar11) {
                lVar7 = 1;
                lVar10 = 1;
                do {
                  if (*(long *)(lVar14 + 8 + lVar10 * 8) == *(long *)(lVar8 + lVar7 * 8)) {
                    lVar1 = lVar10 * 8;
                    lVar2 = lVar10 * 8;
                    lVar10 = lVar10 + 1;
                    *(double *)(lVar12 + lVar7 * 8) =
                         *(double *)(lVar15 + lVar1) * dVar17 + *(double *)(lVar9 + lVar2) * dVar18
                         + *(double *)(lVar12 + lVar7 * 8);
                  }
                } while ((lVar10 <= lVar13) && (lVar7 = lVar7 + 1, lVar7 <= lVar11));
                if (lVar13 < lVar10) goto LAB_009827a9;
              }
              KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0x158);
            }
            else {
              KOBOOLOXSPAIPP(1,"factorapar_ls.c",0x15d);
            }
          }
          else {
            lVar12 = *(long *)(*(long *)(param_4 + 0x80) + (lVar8 - *(long *)(param_4 + 0x88)) * 8);
            lVar11 = *(long *)(lVar4 + lVar8 * 8);
            if (lVar13 != lVar11) goto LAB_00982729;
LAB_00982805:
            lVar8 = 1;
            do {
              *(double *)(lVar12 + lVar8 * 8) =
                   *(double *)(lVar15 + lVar8 * 8) * dVar17 +
                   *(double *)(lVar9 + lVar8 * 8) * dVar18 + *(double *)(lVar12 + lVar8 * 8);
              lVar8 = lVar8 + 1;
            } while (lVar8 <= lVar13);
          }
LAB_009827a9:
        } while ((lVar13 != 1) &&
                (lVar8 = *(long *)(lVar14 + 0x18), lVar11 = lVar9, lVar12 = lVar15,
                lVar14 = lVar14 + 8, lVar8 <= param_3));
      }
    }
  }
  return;
}


