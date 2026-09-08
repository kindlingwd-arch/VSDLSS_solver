/* VSDLSS function (decompiled by Ghidra) */
/* name: getGraphArrFromElemArr_vsdlss  addr: 00a14950  size: 2cd */
#include "vsdlss_ref.h"

/* signature: void getGraphArrFromElemArr_vsdlss(int param_1,int param_2,long param_3,long param_4); */

void getGraphArrFromElemArr_vsdlss(int param_1,int param_2,long param_3,long param_4)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int *piVar11;
  long lVar12;
  long lVar13;
  int iVar14;
  int *piVar15;
  uint uVar16;
  int iVar17;
  long lVar18;
  
  lVar6 = newGraph_vsdlss();
  uVar2 = *(undefined4 *)(param_3 + 8);
  uVar3 = *(undefined4 *)(param_3 + 0xc);
  lVar18 = (long)param_1;
  uVar7 = newIVector_vsdlss((long)(param_2 * 2),0,0);
  *(undefined8 *)(lVar6 + 0x20) = uVar7;
  lVar8 = newIVector_vsdlss(lVar18,1,0);
  *(long *)(lVar6 + 8) = lVar8;
  lVar9 = newIPVector_vsdlss(lVar18,1,0);
  *(long *)(lVar6 + 0x28) = lVar9;
  if (param_4 == 0) {
    lVar12 = newIVector_vsdlss((long)param_2,0,0);
    readIVectorFNum_vsdlss(uVar2,param_2,lVar12);
    lVar13 = newIVector_vsdlss((long)param_2,0,0);
    readIVectorFNum_vsdlss(uVar3,param_2,lVar13);
  }
  else {
    lVar12 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x28);
  }
  checkFromAndToNodes_vsdlss(param_1,param_2,lVar12,lVar13);
  if (0 < param_2) {
    lVar10 = 0;
    do {
      piVar11 = (int *)(lVar8 + (long)*(int *)(lVar12 + 4 + lVar10) * 4);
      *piVar11 = *piVar11 + 1;
      piVar11 = (int *)(lVar13 + 4 + lVar10);
      lVar10 = lVar10 + 4;
      piVar11 = (int *)(lVar8 + (long)*piVar11 * 4);
      *piVar11 = *piVar11 + 1;
    } while (lVar10 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (0 < param_1) {
    *(undefined8 *)(lVar9 + 8) = *(undefined8 *)(lVar6 + 0x20);
    if (param_1 != 1) {
      lVar10 = 0;
      do {
        *(long *)(lVar9 + 0x10 + lVar10 * 2) =
             *(long *)(lVar9 + 8 + lVar10 * 2) + (long)*(int *)(lVar8 + 4 + lVar10) * 4;
        lVar10 = lVar10 + 4;
      } while (lVar10 != (ulong)(param_1 - 2) * 4 + 4);
    }
  }
  initIVector_vsdlss(lVar8,lVar18,0);
  if (0 < param_2) {
    lVar18 = 0;
    do {
      iVar4 = *(int *)(lVar12 + 4 + lVar18);
      iVar14 = *(int *)(lVar13 + 4 + lVar18);
      lVar18 = lVar18 + 4;
      piVar11 = (int *)(lVar8 + (long)iVar4 * 4);
      lVar10 = *(long *)(lVar9 + (long)iVar4 * 8);
      iVar17 = *piVar11 + 1;
      *piVar11 = iVar17;
      *(int *)(lVar10 + (long)iVar17 * 4) = iVar14;
      piVar11 = (int *)(lVar8 + (long)iVar14 * 4);
      lVar10 = *(long *)(lVar9 + (long)iVar14 * 8);
      iVar14 = *piVar11 + 1;
      *piVar11 = iVar14;
      *(int *)(lVar10 + (long)iVar14 * 4) = iVar4;
    } while (lVar18 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (param_4 == 0) {
    if (lVar12 != 0) {
      casiFree_vsdlss(lVar12);
    }
    if (lVar13 != 0) {
      casiFree_vsdlss(lVar13);
    }
  }
  piVar11 = (int *)newCountingMark_vsdlss(param_1);
  lVar18 = *(long *)(piVar11 + 2);
  if (param_1 < 1) {
    uVar16 = 0;
  }
  else {
    uVar16 = 0;
    lVar12 = 0;
    do {
      iVar4 = *(int *)(lVar8 + 4 + lVar12 * 4);
      iVar14 = *piVar11;
      lVar13 = *(long *)(lVar9 + 8 + lVar12 * 8);
      if (iVar4 < 1) {
        iVar17 = 0;
      }
      else {
        piVar15 = (int *)(lVar13 + 4);
        iVar17 = 0;
        do {
          iVar5 = *piVar15;
          piVar1 = (int *)(lVar18 + (long)iVar5 * 4);
          if (*piVar1 < iVar14) {
            iVar17 = iVar17 + 1;
            *piVar1 = iVar14;
            *(int *)(lVar13 + (long)iVar17 * 4) = iVar5;
          }
          piVar15 = piVar15 + 1;
        } while (piVar15 != (int *)(lVar13 + 8 + (ulong)(iVar4 - 1) * 4));
      }
      *(int *)(lVar8 + 4 + lVar12 * 4) = iVar17;
      uVar16 = uVar16 + iVar17;
      unmarkCountingMark_vsdlss(piVar11);
      iVar4 = (int)lVar12;
      lVar12 = lVar12 + 1;
    } while (iVar4 + 2 <= param_1);
    if ((uVar16 & 1) != 0) {
      nrerror_vsdlss(1,"vsdlss1memutils.c",0x74);
    }
  }
  deleteCountingMark_vsdlss(piVar11);
  *(uint *)(lVar6 + 4) = uVar16;
  return;
}


