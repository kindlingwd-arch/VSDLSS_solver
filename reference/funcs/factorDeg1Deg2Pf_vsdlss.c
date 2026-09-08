/* VSDLSS function (decompiled by Ghidra) */
/* name: factorDeg1Deg2Pf_vsdlss  addr: 009e4e80  size: 680 */
#include "vsdlss_ref.h"

/* signature: void factorDeg1Deg2Pf_vsdlss(int param_1,long param_2,long param_3,int *param_4,int *param_5); */

void factorDeg1Deg2Pf_vsdlss(int param_1,long param_2,long param_3,int *param_4,int *param_5)

{
  double dVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double *pdVar14;
  long lVar15;
  long lVar16;
  int iVar17;
  int *piVar18;
  double *pdVar19;
  double *pdVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  double dVar24;
  ulong uVar25;
  
  iVar17 = 1;
  lVar15 = 0;
  iVar2 = *(int *)(param_2 + 4);
  lVar5 = *(long *)(param_2 + 0x28);
  lVar12 = *(long *)(param_2 + 0x30);
  iVar23 = *(int *)(param_2 + 8) + iVar2;
  lVar6 = *(long *)(param_2 + 0x10);
  param_1 = iVar23 + param_1;
  lVar7 = *(long *)(param_2 + 0x38);
  lVar8 = *(long *)(param_2 + 0x40);
  if (0 < iVar2) {
    do {
      if (*(double *)(param_3 + 8 + lVar15 * 2) <= 0.0) {
        nrerror_vsdlss(0x19,"deg123pfutils.c",0x2e1,iVar17);
      }
      iVar22 = *(int *)(lVar5 + 4 + lVar15);
      dVar24 = *(double *)(lVar12 + 8 + lVar15 * 2);
      if (iVar22 != 0) {
        pdVar14 = (double *)(param_3 + (long)iVar22 * 8);
        *pdVar14 = *pdVar14 - (dVar24 * dVar24) / *(double *)(param_3 + 8 + lVar15 * 2);
      }
      iVar17 = iVar17 + 1;
      lVar15 = lVar15 + 4;
    } while (iVar17 <= iVar2);
  }
  iVar17 = iVar2 + 1;
  if (iVar17 <= iVar23) {
    lVar16 = 0;
    lVar15 = (long)iVar17 * 8;
    lVar13 = (long)iVar17 * 4;
    pdVar14 = (double *)(param_3 + lVar15);
    iVar22 = iVar17;
    do {
      if (*pdVar14 <= 0.0) {
        nrerror_vsdlss(0x19,"deg123pfutils.c",0x2fa,iVar22);
      }
      iVar21 = *(int *)(lVar5 + lVar13 + lVar16);
      dVar24 = *(double *)(lVar12 + lVar15 + lVar16 * 2);
      iVar3 = *(int *)(lVar13 + lVar7 + lVar16);
      dVar1 = *(double *)(lVar15 + lVar8 + lVar16 * 2);
      if (iVar21 == 0) {
        if (iVar3 != 0) {
          nrerror_vsdlss(1,"deg123pfutils.c",0x306);
        }
      }
      else {
        pdVar19 = (double *)(param_3 + (long)iVar21 * 8);
        *pdVar19 = *pdVar19 - (dVar24 * dVar24) / *pdVar14;
        if (iVar3 != 0) {
          pdVar19 = (double *)(param_3 + (long)iVar3 * 8);
          *pdVar19 = *pdVar19 - (dVar1 * dVar1) / *pdVar14;
          if (iVar21 == iVar3) {
            nrerror_vsdlss(1,"deg123pfutils.c",0x313);
          }
          lVar10 = ftrwFromNodeVecWrite;
          lVar9 = cmuFromNodeVecWrite;
          iVar4 = iVar21;
          if (iVar3 <= iVar21) {
            iVar4 = iVar3;
          }
          lVar11 = (long)iVar4;
          if (iVar21 <= iVar3) {
            iVar21 = iVar3;
          }
          dVar24 = (dVar24 * dVar1) / *pdVar14;
          uVar25 = (ulong)dVar24 ^ DAT_00abd200;
          if (param_1 < iVar4) {
            *param_5 = *param_5 + 1;
            *(int *)(lVar10 + (long)ftrwCurrentEntryWrite * 4) = iVar4 - param_1;
            *(int *)(ftrwToNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = iVar21 - param_1;
            *(ulong *)(ftrwOffDiagVecWrite + (long)ftrwCurrentEntryWrite * 8) = uVar25;
            ftrwCurrentEntryWrite = ftrwCurrentEntryWrite + 1;
            if (ftrwChunkSizeWrite < ftrwCurrentEntryWrite) {
              writeFromToOffDiag_vsdlss();
              ftrwCurrentEntryWrite = 1;
            }
          }
          else if (iVar23 < iVar4) {
            *param_4 = *param_4 + 1;
            *(int *)(lVar9 + (long)cmuCurrentEntryWrite * 4) = iVar4 - iVar23;
            *(int *)(cmuToNodeVecWrite + (long)cmuCurrentEntryWrite * 4) = iVar21 - iVar23;
            *(ulong *)(cmuOffDiagVecWrite + (long)cmuCurrentEntryWrite * 8) = uVar25;
            cmuCurrentEntryWrite = cmuCurrentEntryWrite + 1;
            if (cmuChunkSizeWrite < cmuCurrentEntryWrite) {
              writeChunkWrite_vsdlss();
              cmuCurrentEntryWrite = 1;
            }
          }
          else {
            piVar18 = (int *)(lVar5 + lVar11 * 4);
            iVar3 = *piVar18;
            if (iVar3 == 0) {
              *piVar18 = iVar21;
              *(ulong *)(lVar12 + lVar11 * 8) = uVar25;
            }
            else if (iVar21 == iVar3) {
              pdVar19 = (double *)(lVar12 + lVar11 * 8);
              *pdVar19 = *pdVar19 - dVar24;
            }
            else {
              piVar18 = (int *)(lVar11 * 4 + lVar7);
              iVar3 = *piVar18;
              if (iVar3 == 0) {
                *piVar18 = iVar21;
                *(ulong *)(lVar8 + lVar11 * 8) = uVar25;
              }
              else if (iVar21 == iVar3) {
                pdVar19 = (double *)(lVar8 + lVar11 * 8);
                *pdVar19 = *pdVar19 - dVar24;
              }
              else {
                nrerror_vsdlss(1,"deg123pfutils.c",0x338);
              }
            }
          }
        }
      }
      iVar22 = iVar22 + 1;
      pdVar14 = pdVar14 + 1;
      lVar16 = lVar16 + 4;
    } while (iVar22 <= iVar23);
  }
  lVar15 = 0;
  if (0 < iVar23) {
    do {
      dVar24 = SQRT(*(double *)(param_3 + 8 + lVar15 * 8));
      if (NAN(dVar24)) {
        dVar24 = sqrt(*(double *)(param_3 + 8 + lVar15 * 8));
      }
      *(double *)(lVar6 + 8 + lVar15 * 8) = dVar24;
      iVar22 = (int)lVar15;
      lVar15 = lVar15 + 1;
    } while (iVar22 + 2 <= iVar23);
  }
  if (0 < iVar2) {
    lVar15 = 0;
    do {
      if (0 < *(int *)(lVar5 + 4 + lVar15)) {
        *(double *)(lVar12 + 8 + lVar15 * 2) =
             *(double *)(lVar12 + 8 + lVar15 * 2) / *(double *)(lVar6 + 8 + lVar15 * 2);
      }
      lVar15 = lVar15 + 4;
    } while (lVar15 != (ulong)(iVar2 - 1) * 4 + 4);
  }
  if (iVar17 <= iVar23) {
    lVar15 = (long)iVar17 * 8;
    lVar13 = (long)iVar17 * 4;
    pdVar14 = (double *)(lVar8 + lVar15);
    pdVar20 = (double *)(lVar6 + lVar15);
    pdVar19 = (double *)(lVar12 + lVar15);
    lVar12 = 0;
    do {
      if (0 < *(int *)(lVar5 + lVar13 + lVar12)) {
        *pdVar19 = *pdVar19 / *pdVar20;
      }
      if (0 < *(int *)(lVar13 + lVar7 + lVar12)) {
        *pdVar14 = *pdVar14 / *pdVar20;
      }
      lVar12 = lVar12 + 4;
      pdVar14 = pdVar14 + 1;
      pdVar20 = pdVar20 + 1;
      pdVar19 = pdVar19 + 1;
    } while (lVar12 != (ulong)((iVar23 - iVar2) - 1) * 4 + 4);
  }
  return;
}


