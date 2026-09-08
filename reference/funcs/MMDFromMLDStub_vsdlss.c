/* VSDLSS function (decompiled by Ghidra) */
/* name: MMDFromMLDStub_vsdlss  addr: 00a2b700  size: 4ce */
#include "vsdlss_ref.h"

/* signature: void MMDFromMLDStub_vsdlss(long param_1,int param_2,undefined8 param_3,long param_4,undefined4 *param_5); */

void MMDFromMLDStub_vsdlss
               (long param_1,int param_2,undefined8 param_3,long param_4,undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  ulong uVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  long local_a8;
  int local_a0;
  long local_88;
  int local_80;
  int local_6c;
  int *local_58;
  undefined4 local_40;
  undefined4 local_3c [3];
  
  iVar1 = *(int *)(param_1 + 8);
  uVar7 = newCountingMark_vsdlss(iVar1);
  uVar2 = *param_5;
  lVar4 = *(long *)(param_5 + 8);
  lVar8 = (long)iVar1;
  lVar9 = newIVector_vsdlss(lVar8,0,0);
  if (iVar1 < 0x400) {
    iVar5 = sumIVectorElems_vsdlss(*(undefined8 *)(param_1 + 0x28),iVar1);
    local_80 = (iVar5 * 3) / iVar1;
    if (local_80 < 9) {
      local_80 = 9;
    }
    local_58 = (int *)newIVector_vsdlss(lVar8,0,0);
    copyIVector_vsdlss(param_4,lVar8,local_58);
    local_88 = newCVector_vsdlss(lVar8,0,0);
    markAndMoveToEnd(iVar1,param_2,param_3,local_58,local_88);
    local_a8 = newIVector_vsdlss(lVar8,0,0);
    initIVector_vsdlss(local_a8,lVar8,0);
    initIVector_vsdlss(param_4,lVar8,0x8000000c);
    local_40 = 0;
    if (iVar1 < 1) {
      local_a0 = 0;
      goto LAB_00a2b9f5;
    }
    local_6c = 1;
  }
  else {
    local_6c = iVar1 >> 10;
    iVar5 = sumIVectorElems_vsdlss(*(undefined8 *)(param_1 + 0x28),iVar1);
    local_80 = (iVar5 * 3) / iVar1;
    if (local_80 < 9) {
      local_80 = 9;
    }
    local_58 = (int *)newIVector_vsdlss(lVar8,0,0);
    copyIVector_vsdlss(param_4,lVar8,local_58);
    local_88 = newCVector_vsdlss(lVar8,0,0);
    markAndMoveToEnd(iVar1,param_2,param_3,local_58,local_88);
    local_a8 = newIVector_vsdlss(lVar8,0,0);
    initIVector_vsdlss(local_a8,lVar8,0);
    initIVector_vsdlss(param_4,lVar8,0x8000000c);
  }
  local_40 = 0;
  lVar16 = 0;
  iVar15 = 0;
  iVar5 = 1;
  local_a0 = 0;
  piVar13 = local_58;
  do {
    piVar13 = piVar13 + 1;
    if ((iVar1 - param_2) + 1 == iVar5) {
      initCVector_vsdlss(local_88,lVar8,0);
    }
    iVar3 = *piVar13;
    if (*(int *)(param_4 + (long)iVar3 * 4) < -0x3ffffff9) {
      if (0 < *(int *)(local_a8 + (long)iVar3 * 4)) {
        MLDUpdateNodeAdjArray_vsdlss(iVar3,param_4,param_1,local_a8,uVar7);
      }
      iVar6 = MLDFindReachArrRetSize(iVar3,param_4,param_1,lVar9,uVar7);
      iVar12 = iVar6 + 1;
      lVar10 = lVar16;
      if (iVar15 < iVar12) {
        iVar14 = iVar6 + 2;
        if (iVar12 < local_6c) {
          iVar14 = local_6c;
        }
        iVar15 = iVar14 + -1;
        lVar10 = newBlock_vsdlss(uVar2,(long)iVar14 << 2);
      }
      iVar15 = iVar15 - iVar12;
      lVar16 = lVar10 + (long)iVar12 * 4;
      *(int *)(lVar10 + 4) = iVar3;
      uVar11 = 1;
      if ((long)iVar6 != 0) {
        do {
          *(undefined4 *)(lVar10 + 4 + uVar11 * 4) = *(undefined4 *)(lVar9 + uVar11 * 4);
          uVar11 = uVar11 + 1;
        } while (uVar11 <= (ulong)(long)iVar6);
      }
      if (local_80 < iVar6) {
        MLDElimIndistNodesArrStubCombo
                  (iVar3,local_3c,iVar6,lVar10 + 4,param_4,param_1,local_a8,&local_40,uVar7,local_88
                  );
      }
      else {
        local_3c[0] = 1;
        MLDElimIndistNodesArrStub_1
                  (iVar3,1,iVar6,lVar10 + 4,param_4,param_1,local_a8,&local_40,uVar7);
      }
      local_a0 = local_a0 + 1;
      *(undefined4 *)(lVar4 + (long)local_a0 * 4) = local_3c[0];
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 <= iVar1);
LAB_00a2b9f5:
  MLDFinishReordering_vsdlss(iVar1,local_40,param_4,uVar7);
  checkOrderedAtEnd(iVar1,param_2,param_3,param_4);
  if (local_a8 != 0) {
    casiFree_vsdlss();
  }
  if (local_88 != 0) {
    casiFree_vsdlss();
  }
  if (local_58 != (int *)0x0) {
    casiFree_vsdlss();
  }
  deleteCountingMark_vsdlss(uVar7);
  if (lVar9 != 0) {
    casiFree_vsdlss(lVar9);
  }
  param_5[6] = 1;
  param_5[5] = local_a0;
  param_5[7] = local_a0;
  return;
}


