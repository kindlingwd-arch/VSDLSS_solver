/* VSDLSS function (decompiled by Ghidra) */
/* name: formMatForDeg3_vsdlss  addr: 009e4b00  size: 33c */
#include "vsdlss_ref.h"

/* signature: long formMatForDeg3_vsdlss(undefined4 param_1,undefined4 param_2,int param_3,int param_4); */

long formMatForDeg3_vsdlss(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  int *piVar12;
  int iVar13;
  int iVar14;
  double dVar15;
  
  iVar8 = enqSolveInMem_vsdlss();
  if (iVar8 == 0) {
    lVar9 = newDeg3Pf_vsdlss(param_2,param_3);
  }
  else {
    lVar9 = tagNewDeg3Pf_vsdlss(param_1,param_2,param_3);
  }
  lVar2 = *(long *)(lVar9 + 0x20);
  lVar3 = *(long *)(lVar9 + 0x10);
  lVar4 = *(long *)(lVar9 + 0x18);
  lVar5 = *(long *)(lVar9 + 0x28);
  lVar6 = *(long *)(lVar9 + 0x30);
  lVar7 = *(long *)(lVar9 + 0x38);
  initChunkRead_vsdlss(0xd);
  cmuCurrentEntryRead = cmuCurrentEntryRead + 1;
  if (cmuChunkSizeRead < cmuCurrentEntryRead) {
    cmuCurrentEntryRead = 1;
    cmuNumInCurrentChunk = readChunkRead_vsdlss();
    if (cmuChunkSizeRead < cmuNumInCurrentChunk) {
      nrerror_vsdlss(1,"deg123pfutils.c",0x285);
    }
    if (cmuNumInCurrentChunk == 0) goto LAB_009e4ca9;
  }
  else if (cmuNumInCurrentChunk < cmuCurrentEntryRead) {
LAB_009e4ca9:
    iVar13 = 0;
    iVar8 = cmuNumInCurrentChunk;
LAB_009e4c44:
    cmuCurrentEntryRead = iVar8;
    cmuNumInCurrentChunk = cmuCurrentEntryRead;
    finishChunkRead_vsdlss();
    if (iVar13 != param_4) {
      nrerror_vsdlss(1,"deg123pfutils.c",0x2ae);
    }
    return lVar9;
  }
  lVar11 = (long)cmuCurrentEntryRead;
  iVar13 = 1;
  iVar8 = *(int *)(cmuFromNodeVecRead + lVar11 * 4);
  iVar14 = *(int *)(cmuToNodeVecRead + lVar11 * 4);
  dVar15 = *(double *)(cmuOffDiagVecRead + lVar11 * 8);
  if (param_3 < iVar8) goto LAB_009e4d19;
  do {
    if (iVar14 <= iVar8) goto LAB_009e4d19;
    while( true ) {
      lVar10 = (long)iVar8;
      lVar11 = lVar10 * 4;
      piVar12 = (int *)(lVar3 + lVar11);
      iVar8 = *piVar12;
      if (iVar8 == 0) {
        *piVar12 = iVar14;
        *(double *)(lVar4 + lVar10 * 8) = dVar15;
      }
      else if (iVar14 == iVar8) {
        pdVar1 = (double *)(lVar4 + lVar10 * 8);
        *pdVar1 = dVar15 + *pdVar1;
      }
      else {
        piVar12 = (int *)(lVar2 + lVar11);
        iVar8 = *piVar12;
        if (iVar8 == 0) {
          *piVar12 = iVar14;
          *(double *)(lVar5 + lVar10 * 8) = dVar15;
        }
        else if (iVar14 == iVar8) {
          pdVar1 = (double *)(lVar5 + lVar10 * 8);
          *pdVar1 = dVar15 + *pdVar1;
        }
        else {
          piVar12 = (int *)(lVar11 + lVar6);
          iVar8 = *piVar12;
          if (iVar8 == 0) {
            *piVar12 = iVar14;
            *(double *)(lVar7 + lVar10 * 8) = dVar15;
          }
          else if (iVar14 == iVar8) {
            pdVar1 = (double *)(lVar7 + lVar10 * 8);
            *pdVar1 = dVar15 + *pdVar1;
          }
          else {
            nrerror_vsdlss(1,"deg123pfutils.c",0x2a2);
          }
        }
      }
      cmuCurrentEntryRead = cmuCurrentEntryRead + 1;
      if (cmuChunkSizeRead < cmuCurrentEntryRead) {
        cmuCurrentEntryRead = 1;
        cmuNumInCurrentChunk = readChunkRead_vsdlss();
        if (cmuChunkSizeRead < cmuNumInCurrentChunk) {
          nrerror_vsdlss(1,"deg123pfutils.c",0x2a9);
        }
        iVar8 = cmuNumInCurrentChunk;
        if (cmuNumInCurrentChunk == 0) goto LAB_009e4c44;
      }
      else {
        iVar8 = cmuNumInCurrentChunk;
        if (cmuNumInCurrentChunk < cmuCurrentEntryRead) goto LAB_009e4c44;
      }
      lVar11 = (long)cmuCurrentEntryRead;
      iVar13 = iVar13 + 1;
      iVar8 = *(int *)(cmuFromNodeVecRead + lVar11 * 4);
      iVar14 = *(int *)(cmuToNodeVecRead + lVar11 * 4);
      dVar15 = *(double *)(cmuOffDiagVecRead + lVar11 * 8);
      if (iVar8 <= param_3) break;
LAB_009e4d19:
      nrerror_vsdlss(1,"deg123pfutils.c",0x289);
    }
  } while( true );
}


