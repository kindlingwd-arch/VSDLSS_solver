/* VSDLSS function (decompiled by Ghidra) */
/* name: formMatForDeg1Deg2AndSplit_vsdlss  addr: 009e4620  size: 4a2 */
#include "vsdlss_ref.h"

/* signature: long formMatForDeg1Deg2AndSplit_vsdlss(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,undefined4 param_5,int param_6,long param_7,int *param_8,int *param_9); */

long formMatForDeg1Deg2AndSplit_vsdlss
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,
               undefined4 param_5,int param_6,long param_7,int *param_8,int *param_9)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  int *piVar9;
  int *piVar10;
  int iVar11;
  double *pdVar12;
  long lVar13;
  double *pdVar14;
  undefined8 uVar15;
  long local_48;
  
  iVar7 = enqSolveInMem_vsdlss();
  if (iVar7 == 0) {
    local_48 = newDeg1Deg2PfNoDiagVec_vsdlss(param_2,param_4,param_5);
  }
  else {
    local_48 = tagNewDeg1Deg2PfNoDiagVec_vsdlss(param_1,param_2,param_4,param_5);
  }
  lVar3 = *(long *)(local_48 + 0x38);
  lVar4 = *(long *)(local_48 + 0x28);
  iVar11 = *(int *)(local_48 + 4) + *(int *)(local_48 + 8);
  lVar5 = *(long *)(local_48 + 0x30);
  lVar6 = *(long *)(local_48 + 0x40);
  initReadFromToOffDiag_vsdlss(0x1b,0x1c,0x1d,param_3);
  iVar7 = ftrwNumItemsLeft;
  if (ftrwNumItemsLeft < 1) {
LAB_009e4810:
    ftrwNumItemsLeft = iVar7;
    finishReadFromToOffDiag_vsdlss();
    if (param_4 + 1 <= iVar11) {
      lVar13 = (long)(param_4 + 1);
      piVar9 = (int *)(lVar4 + lVar13 * 4);
      pdVar14 = (double *)(lVar5 + lVar13 * 8);
      piVar10 = (int *)(lVar13 * 4 + lVar3);
      pdVar12 = (double *)(lVar13 * 8 + lVar6);
      do {
        if ((*piVar9 != 0) && (*piVar9 == *piVar10)) {
          *pdVar14 = *pdVar14 + *pdVar12;
          *piVar10 = 0;
          *pdVar12 = 0.0;
        }
        piVar9 = piVar9 + 1;
        piVar10 = piVar10 + 1;
        pdVar14 = pdVar14 + 1;
        pdVar12 = pdVar12 + 1;
      } while (piVar9 != (int *)(lVar4 + 4 + (lVar13 + (ulong)(uint)((iVar11 + -1) - param_4)) * 4))
      ;
    }
    return local_48;
  }
  ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
  if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
    readFromToOffDiag_vsdlss();
    ftrwCurrentEntryRead = 1;
    lVar8 = 8;
    lVar13 = 4;
  }
  else {
    lVar13 = (long)ftrwCurrentEntryRead * 4;
    lVar8 = (long)ftrwCurrentEntryRead << 3;
  }
  iVar7 = *(int *)(ftrwFromNodeVecRead + lVar13);
  iVar1 = *(int *)(ftrwToNodeVecRead + lVar13);
  uVar15 = *(undefined8 *)(ftrwOffDiagVecRead + lVar8);
  param_6 = param_6 + iVar11;
  do {
    ftrwNumItemsLeft = ftrwNumItemsLeft + -1;
    iVar2 = *(int *)(param_7 + (long)iVar7 * 4);
    iVar1 = *(int *)(param_7 + (long)iVar1 * 4);
    if (iVar2 == iVar1) {
      nrerror_vsdlss(0x2c,"deg123pfutils.c",0x213,iVar7);
LAB_009e48ce:
      iVar7 = iVar1;
      iVar1 = iVar2;
    }
    else {
      iVar7 = iVar2;
      if (iVar2 <= iVar1) goto LAB_009e48ce;
    }
    lVar13 = (long)iVar1;
    if (param_4 < iVar1) {
      if (iVar11 < iVar1) {
        if (param_6 < iVar1) {
          *param_9 = *param_9 + 1;
          *(int *)(ftrwFromNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = iVar1 - param_6;
          *(int *)(ftrwToNodeVecWrite + (long)ftrwCurrentEntryWrite * 4) = iVar7 - param_6;
          *(undefined8 *)(ftrwOffDiagVecWrite + (long)ftrwCurrentEntryWrite * 8) = uVar15;
          ftrwCurrentEntryWrite = ftrwCurrentEntryWrite + 1;
          iVar1 = ftrwNumItemsLeft;
          iVar7 = ftrwNumItemsLeft;
          if (ftrwChunkSizeWrite < ftrwCurrentEntryWrite) {
            writeFromToOffDiag_vsdlss();
            ftrwCurrentEntryWrite = 1;
            iVar1 = ftrwNumItemsLeft;
            iVar7 = ftrwNumItemsLeft;
          }
        }
        else {
          *param_8 = *param_8 + 1;
          *(int *)(cmuFromNodeVecWrite + (long)cmuCurrentEntryWrite * 4) = iVar1 - iVar11;
          *(int *)(cmuToNodeVecWrite + (long)cmuCurrentEntryWrite * 4) = iVar7 - iVar11;
          *(undefined8 *)(cmuOffDiagVecWrite + (long)cmuCurrentEntryWrite * 8) = uVar15;
          cmuCurrentEntryWrite = cmuCurrentEntryWrite + 1;
          iVar1 = ftrwNumItemsLeft;
          iVar7 = ftrwNumItemsLeft;
          if (cmuChunkSizeWrite < cmuCurrentEntryWrite) {
            writeChunkWrite_vsdlss();
            cmuCurrentEntryWrite = 1;
            iVar1 = ftrwNumItemsLeft;
            iVar7 = ftrwNumItemsLeft;
          }
        }
      }
      else {
        piVar9 = (int *)(lVar4 + lVar13 * 4);
        if ((*piVar9 == 0) ||
           ((piVar10 = (int *)(lVar3 + lVar13 * 4), *piVar10 != 0 &&
            (nrerror_vsdlss(1,"deg123pfutils.c",0x225), *piVar9 == 0)))) goto LAB_009e4726;
        *piVar10 = iVar7;
        iVar1 = ftrwNumItemsLeft;
        *(undefined8 *)(lVar6 + lVar13 * 8) = uVar15;
        iVar7 = ftrwNumItemsLeft;
      }
    }
    else {
      piVar9 = (int *)(lVar4 + lVar13 * 4);
      if (*piVar9 != 0) {
        nrerror_vsdlss(1,"deg123pfutils.c",0x21d);
      }
LAB_009e4726:
      *piVar9 = iVar7;
      *(undefined8 *)(lVar5 + lVar13 * 8) = uVar15;
      iVar1 = ftrwNumItemsLeft;
      iVar7 = ftrwNumItemsLeft;
    }
    ftrwNumItemsLeft = iVar1;
    if (ftrwNumItemsLeft < 1) goto LAB_009e4810;
    ftrwCurrentEntryRead = ftrwCurrentEntryRead + 1;
    if (ftrwChunkSizeRead < ftrwCurrentEntryRead) {
      ftrwNumItemsLeft = iVar7;
      readFromToOffDiag_vsdlss();
      ftrwCurrentEntryRead = 1;
      lVar8 = 8;
      lVar13 = 4;
    }
    else {
      lVar13 = (long)ftrwCurrentEntryRead * 4;
      lVar8 = (long)ftrwCurrentEntryRead << 3;
    }
    iVar7 = *(int *)(ftrwFromNodeVecRead + lVar13);
    iVar1 = *(int *)(ftrwToNodeVecRead + lVar13);
    uVar15 = *(undefined8 *)(ftrwOffDiagVecRead + lVar8);
  } while( true );
}


