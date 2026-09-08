/* VSDLSS function (decompiled by Ghidra) */
/* name: getMLDGraphFromElemArr_vsdlss  addr: 00a060d0  size: 34e */
#include "vsdlss_ref.h"

/* signature: void getMLDGraphFromElemArr_vsdlss(int param_1,int param_2,long param_3,long param_4,undefined4 *param_5); */

void getMLDGraphFromElemArr_vsdlss
               (int param_1,int param_2,long param_3,long param_4,undefined4 *param_5)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  long lVar15;
  int local_48 [2];
  long local_40;
  
  lVar15 = (long)param_1;
  uVar2 = *(undefined4 *)(param_3 + 8);
  uVar3 = *(undefined4 *)(param_3 + 0xc);
  *param_5 = 0xffffffff;
  param_5[3] = 0;
  *(undefined8 *)(param_5 + 10) = 0;
  *(undefined8 *)(param_5 + 0xc) = 0;
  *(undefined8 *)(param_5 + 0xe) = 0;
  *(undefined8 *)(param_5 + 0x10) = 0;
  *(undefined8 *)(param_5 + 0x12) = 0;
  *(undefined8 *)(param_5 + 0x14) = 0;
  *(undefined8 *)(param_5 + 0x16) = 0;
  *(undefined8 *)(param_5 + 0x18) = 0;
  param_5[1] = 1;
  param_5[2] = param_1;
  uVar4 = newIVector_vsdlss((long)(param_2 * 2),0,0);
  *(undefined8 *)(param_5 + 0x10) = uVar4;
  lVar5 = newIVector_vsdlss(lVar15,1,0);
  *(long *)(param_5 + 10) = lVar5;
  lVar6 = newIPVector_vsdlss(lVar15,1,0);
  *(long *)(param_5 + 0xe) = lVar6;
  if (param_4 == 0) {
    lVar8 = newIVector_vsdlss((long)param_2,0,0);
    readIVectorFNum_vsdlss(uVar2,param_2,lVar8);
    lVar9 = newIVector_vsdlss((long)param_2,0,0);
    readIVectorFNum_vsdlss(uVar3,param_2,lVar9);
  }
  else {
    lVar8 = *(long *)(param_4 + 0x18);
    lVar9 = *(long *)(param_4 + 0x28);
  }
  checkFromAndToNodes_vsdlss(param_1,param_2,lVar8,lVar9);
  if (0 < param_2) {
    lVar7 = 0;
    do {
      piVar11 = (int *)(lVar5 + (long)*(int *)(lVar8 + 4 + lVar7) * 4);
      *piVar11 = *piVar11 + 1;
      piVar11 = (int *)(lVar9 + 4 + lVar7);
      lVar7 = lVar7 + 4;
      piVar11 = (int *)(lVar5 + (long)*piVar11 * 4);
      *piVar11 = *piVar11 + 1;
    } while (lVar7 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (0 < param_1) {
    *(undefined8 *)(lVar6 + 8) = *(undefined8 *)(param_5 + 0x10);
    if (param_1 != 1) {
      lVar7 = 0;
      do {
        *(long *)(lVar6 + 0x10 + lVar7 * 2) =
             *(long *)(lVar6 + 8 + lVar7 * 2) + (long)*(int *)(lVar5 + 4 + lVar7) * 4;
        lVar7 = lVar7 + 4;
      } while (lVar7 != (ulong)(param_1 - 2) * 4 + 4);
    }
  }
  initIVector_vsdlss(lVar5,lVar15,0);
  if (0 < param_2) {
    lVar15 = 0;
    do {
      iVar14 = *(int *)(lVar8 + 4 + lVar15);
      iVar10 = *(int *)(lVar9 + 4 + lVar15);
      lVar15 = lVar15 + 4;
      piVar11 = (int *)(lVar5 + (long)iVar14 * 4);
      lVar7 = *(long *)(lVar6 + (long)iVar14 * 8);
      iVar12 = *piVar11 + 1;
      *piVar11 = iVar12;
      *(int *)(lVar7 + (long)iVar12 * 4) = iVar10;
      piVar11 = (int *)(lVar5 + (long)iVar10 * 4);
      lVar7 = *(long *)(lVar6 + (long)iVar10 * 8);
      iVar10 = *piVar11 + 1;
      *piVar11 = iVar10;
      *(int *)(lVar7 + (long)iVar10 * 4) = iVar14;
    } while (lVar15 != (ulong)(param_2 - 1) * 4 + 4);
  }
  if (param_4 == 0) {
    if (lVar8 != 0) {
      casiFree_vsdlss(lVar8);
    }
    if (lVar9 != 0) {
      casiFree_vsdlss(lVar9);
    }
  }
  lVar15 = *(long *)(param_5 + 0x10);
  newCountingMarkNoHeader(local_48);
  if (param_1 < 1) {
    iVar14 = 0;
  }
  else {
    uVar13 = 0;
    lVar8 = 0;
    do {
      iVar14 = *(int *)(lVar5 + 4 + lVar8 * 4);
      lVar7 = *(long *)(lVar6 + 8 + lVar8 * 8);
      lVar9 = lVar15 + (long)(int)uVar13 * 4;
      if (iVar14 < 1) {
        iVar10 = 0;
      }
      else {
        piVar11 = (int *)(lVar7 + 4);
        iVar10 = 0;
        do {
          iVar12 = *piVar11;
          piVar1 = (int *)(local_40 + (long)iVar12 * 4);
          if (*piVar1 < local_48[0]) {
            iVar10 = iVar10 + 1;
            *piVar1 = local_48[0];
            *(int *)(lVar9 + (long)iVar10 * 4) = iVar12;
          }
          piVar11 = piVar11 + 1;
        } while (piVar11 != (int *)(lVar7 + 8 + (ulong)(iVar14 - 1) * 4));
      }
      *(int *)(lVar5 + 4 + lVar8 * 4) = iVar10;
      *(long *)(lVar6 + 8 + lVar8 * 8) = lVar9;
      uVar13 = uVar13 + iVar10;
      unmarkCountingMark_vsdlss(local_48);
      iVar14 = (int)lVar8;
      lVar8 = lVar8 + 1;
    } while (iVar14 + 2 <= param_1);
    if ((uVar13 & 1) != 0) {
      nrerror_vsdlss(1,"mldorder.c",0x164a);
    }
    iVar14 = (int)uVar13 >> 1;
  }
  param_5[3] = iVar14;
  deleteCountingMarkNoHeader(local_48);
  return;
}


