/* VSDLSS function (decompiled by Ghidra) */
/* name: initFactorOneChunk_vsdlss  addr: 00a16410  size: 2d7 */
#include "vsdlss_ref.h"

/* signature: void initFactorOneChunk_vsdlss(int param_1,long param_2,long param_3,long param_4); */

void initFactorOneChunk_vsdlss(int param_1,long param_2,long param_3,long param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  double *pdVar9;
  long lVar10;
  long lVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  double dVar17;
  
  uVar1 = *(undefined4 *)(param_2 + 8);
  uVar2 = *(undefined4 *)(param_2 + 0xc);
  uVar3 = *(undefined4 *)(param_2 + 0x10);
  lVar4 = *(long *)(param_4 + 0x30);
  lVar5 = *(long *)(param_4 + 0x38);
  lVar6 = *(long *)(param_4 + 0x28);
  readDiagVecFNumPermuteIntoFc(param_3,*(undefined4 *)(param_2 + 4));
  iVar15 = 0;
  initReadFromToOffDiag_vsdlss(uVar1,uVar2,uVar3,param_1);
  if (0 < ftrwNumItemsLeft) {
    ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
    if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
      readFromToOffDiag_vsdlss();
      ftrwCurrentEntryRead = 1;
      lVar16 = 8;
      lVar10 = 4;
    }
    else {
      lVar10 = (long)ftrwCurrentEntryRead * 4;
      lVar16 = (long)ftrwCurrentEntryRead * 8;
    }
    iVar15 = 0;
    iVar13 = *(int *)(ftrwFromNodeVecRead + lVar10);
    iVar12 = *(int *)(ftrwToNodeVecRead + lVar10);
    dVar17 = *(double *)(ftrwOffDiagVecRead + lVar16);
    do {
      ftrwNumItemsLeft = ftrwNumItemsLeft + -1;
      iVar12 = *(int *)(param_3 + (long)iVar12 * 4);
      iVar15 = iVar15 + 1;
      iVar13 = *(int *)(param_3 + (long)iVar13 * 4);
      iVar14 = iVar13;
      if (iVar13 <= iVar12) {
        iVar14 = iVar12;
        iVar12 = iVar13;
      }
      lVar16 = (long)iVar12;
      iVar13 = *(int *)(lVar6 + lVar16 * 4);
      lVar10 = *(long *)(lVar4 + lVar16 * 8);
      if (iVar13 < 0x21) {
        if (0 < iVar13) {
          lVar7 = 2;
          if (*(int *)(lVar10 + 4) == iVar14) {
            lVar11 = 8;
            iVar12 = 1;
          }
          else {
            do {
              lVar11 = lVar7;
              iVar12 = (int)lVar11;
              if (iVar13 < iVar12) goto LAB_00a16538;
              lVar7 = lVar11 + 1;
            } while (*(int *)(lVar10 + -4 + (lVar11 + 1) * 4) != iVar14);
            lVar11 = lVar11 << 3;
          }
          pdVar9 = (double *)(lVar11 + *(long *)(lVar5 + lVar16 * 8));
          *pdVar9 = dVar17 + *pdVar9;
          iVar14 = ftrwNumItemsLeft;
          if (iVar12 <= iVar13) goto joined_r0x00a16636;
        }
LAB_00a16538:
        nrerror_vsdlss(1,"vsdlss1memutils.c",0x310);
        iVar14 = ftrwNumItemsLeft;
      }
      else {
        iVar12 = 1;
        do {
          while (iVar8 = (iVar12 + iVar13) / 2, *(int *)(lVar10 + (long)iVar8 * 4) < iVar14) {
            iVar12 = iVar8 + 1;
            if (iVar13 <= iVar12) goto LAB_00a165ea;
          }
          iVar13 = iVar8;
        } while (iVar12 < iVar8);
LAB_00a165ea:
        if ((iVar12 != iVar13) || (*(int *)(lVar10 + (long)iVar12 * 4) != iVar14)) {
          nrerror_vsdlss(1,"vsdlss1memutils.c",0x31d);
        }
        iVar14 = ftrwNumItemsLeft;
        pdVar9 = (double *)(*(long *)(lVar5 + lVar16 * 8) + (long)iVar12 * 8);
        *pdVar9 = dVar17 + *pdVar9;
      }
joined_r0x00a16636:
      if (iVar14 < 1) break;
      ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
      if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
        readFromToOffDiag_vsdlss();
        ftrwCurrentEntryRead = 1;
        lVar16 = 8;
        lVar10 = 4;
      }
      else {
        lVar10 = (long)ftrwCurrentEntryRead * 4;
        lVar16 = (long)ftrwCurrentEntryRead * 8;
        ftrwNumItemsLeft = iVar14;
      }
      iVar13 = *(int *)(ftrwFromNodeVecRead + lVar10);
      iVar12 = *(int *)(ftrwToNodeVecRead + lVar10);
      dVar17 = *(double *)(ftrwOffDiagVecRead + lVar16);
    } while( true );
  }
  if (param_1 != iVar15) {
    nrerror_vsdlss(1,"vsdlss1memutils.c",0x328);
  }
  finishReadFromToOffDiag_vsdlss();
  return;
}


