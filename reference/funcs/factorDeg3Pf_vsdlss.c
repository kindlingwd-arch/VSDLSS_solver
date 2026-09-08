/* VSDLSS function (decompiled by Ghidra) */
/* name: factorDeg3Pf_vsdlss  addr: 009e5540  size: 575 */
#include "vsdlss_ref.h"

/* signature: void factorDeg3Pf_vsdlss(long param_1,long param_2,int *param_3); */

void factorDeg3Pf_vsdlss(long param_1,long param_2,int *param_3)

{
  double *pdVar1;
  double __x;
  double dVar2;
  double dVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  long lVar20;
  double dVar21;
  ulong uVar22;
  
  iVar15 = 1;
  lVar7 = *(long *)(param_1 + 0x10);
  iVar5 = *(int *)(param_1 + 4);
  lVar8 = *(long *)(param_1 + 0x18);
  lVar9 = *(long *)(param_1 + 0x20);
  lVar10 = *(long *)(param_1 + 0x28);
  lVar11 = *(long *)(param_1 + 0x30);
  lVar12 = *(long *)(param_1 + 0x38);
  lVar13 = *(long *)(param_1 + 8);
  lVar20 = 0;
  if (0 < iVar5) {
    do {
      __x = *(double *)(param_2 + 8 + lVar20 * 8);
      if (__x <= 0.0) {
        nrerror_vsdlss(__x,0x19,"deg123pfutils.c",0x378,iVar15);
      }
      dVar2 = *(double *)(lVar8 + 8 + lVar20 * 8);
      iVar15 = *(int *)(lVar7 + 4 + lVar20 * 4);
      iVar19 = *(int *)(lVar9 + 4 + lVar20 * 4);
      dVar3 = *(double *)(lVar10 + 8 + lVar20 * 8);
      iVar6 = *(int *)(lVar11 + 4 + lVar20 * 4);
      dVar4 = *(double *)(lVar12 + 8 + lVar20 * 8);
      if (((iVar15 == 0) && (iVar6 != 0 || iVar19 != 0)) || ((iVar6 != 0 && (iVar19 == 0)))) {
        nrerror_vsdlss(1,"deg123pfutils.c",0x387);
      }
      dVar21 = SQRT(__x);
      if (NAN(dVar21)) {
        dVar21 = sqrt(__x);
      }
      *(double *)(lVar13 + 8 + lVar20 * 8) = dVar21;
      if (iVar15 != 0) {
        if (iVar15 <= iVar5) {
          nrerror_vsdlss(1,"deg123pfutils.c",0x391);
        }
        pdVar1 = (double *)(param_2 + (long)iVar15 * 8);
        *pdVar1 = *pdVar1 - (dVar2 * dVar2) / __x;
        *(double *)(lVar8 + 8 + lVar20 * 8) = *(double *)(lVar8 + 8 + lVar20 * 8) / dVar21;
        if (iVar19 != 0) {
          if (iVar19 <= iVar5) {
            nrerror_vsdlss(1,"deg123pfutils.c",0x39a);
          }
          lVar17 = ftrwFromNodeVecWrite;
          pdVar1 = (double *)(param_2 + (long)iVar19 * 8);
          *pdVar1 = *pdVar1 - (dVar3 * dVar3) / __x;
          *(double *)(lVar10 + 8 + lVar20 * 8) = *(double *)(lVar10 + 8 + lVar20 * 8) / dVar21;
          *param_3 = *param_3 + 1;
          iVar14 = iVar15;
          if (iVar19 <= iVar15) {
            iVar14 = iVar19;
          }
          *(int *)(lVar17 + (long)ftrwCurrentEntryWrite * 4) = iVar14 - iVar5;
          iVar14 = iVar15;
          if (iVar15 <= iVar19) {
            iVar14 = iVar19;
          }
          *(int *)(ftrwToNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = iVar14 - iVar5;
          *(ulong *)(ftrwOffDiagVecWrite + (long)ftrwCurrentEntryWrite * 8) =
               (ulong)((dVar3 * dVar2) / __x) ^ DAT_00abd200;
          ftrwCurrentEntryWrite = ftrwCurrentEntryWrite + 1;
          if (ftrwChunkSizeWrite < ftrwCurrentEntryWrite) {
            writeFromToOffDiag_vsdlss();
            ftrwCurrentEntryWrite = 1;
          }
          if (iVar6 != 0) {
            if (iVar6 <= iVar5) {
              nrerror_vsdlss(1,"deg123pfutils.c",0x3ae);
            }
            lVar17 = ftrwFromNodeVecWrite;
            pdVar1 = (double *)(param_2 + (long)iVar6 * 8);
            uVar22 = (ulong)((dVar2 * dVar4) / __x) ^ DAT_00abd200;
            *pdVar1 = *pdVar1 - (dVar4 * dVar4) / __x;
            *(double *)(lVar12 + 8 + lVar20 * 8) = *(double *)(lVar12 + 8 + lVar20 * 8) / dVar21;
            *param_3 = *param_3 + 1;
            iVar14 = iVar15;
            if (iVar6 <= iVar15) {
              iVar14 = iVar6;
            }
            if (iVar15 <= iVar6) {
              iVar15 = iVar6;
            }
            *(int *)(lVar17 + (long)ftrwCurrentEntryWrite * 4) = iVar14 - iVar5;
            lVar18 = ftrwToNodeVecWrite;
            *(int *)(ftrwToNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = iVar15 - iVar5;
            lVar16 = ftrwOffDiagVecWrite;
            *(ulong *)(ftrwOffDiagVecWrite + (long)ftrwCurrentEntryWrite * 8) = uVar22;
            ftrwCurrentEntryWrite = ftrwCurrentEntryWrite + 1;
            if (ftrwChunkSizeWrite < ftrwCurrentEntryWrite) {
              writeFromToOffDiag_vsdlss();
              ftrwCurrentEntryWrite = 1;
              lVar16 = ftrwOffDiagVecWrite;
              lVar17 = ftrwFromNodeVecWrite;
              lVar18 = ftrwToNodeVecWrite;
            }
            *param_3 = *param_3 + 1;
            iVar15 = iVar19;
            if (iVar6 <= iVar19) {
              iVar15 = iVar6;
            }
            if (iVar19 <= iVar6) {
              iVar19 = iVar6;
            }
            *(int *)(lVar17 + (long)ftrwCurrentEntryWrite * 4) = iVar15 - iVar5;
            *(int *)(lVar18 + (long)ftrwCurrentEntryWrite * 4) = iVar19 - iVar5;
            *(ulong *)(lVar16 + (long)ftrwCurrentEntryWrite * 8) =
                 (ulong)((dVar3 * dVar4) / __x) ^ DAT_00abd200;
            ftrwCurrentEntryWrite = ftrwCurrentEntryWrite + 1;
            if (ftrwChunkSizeWrite < ftrwCurrentEntryWrite) {
              writeFromToOffDiag_vsdlss();
              ftrwCurrentEntryWrite = 1;
            }
          }
        }
      }
      iVar15 = (int)lVar20 + 2;
      lVar20 = lVar20 + 1;
    } while (iVar15 <= iVar5);
  }
  return;
}


