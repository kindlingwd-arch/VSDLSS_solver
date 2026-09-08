/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDInitPartition_vsdlss  addr: 00a009c0  size: 211 */
#include "vsdlss_ref.h"

/* signature: void MLDInitPartition_vsdlss(undefined8 param_1,long param_2,long param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void MLDInitPartition_vsdlss(undefined8 param_1,long param_2,long param_3,long param_4)

{
  int *piVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  int *piVar8;
  long lVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  long lVar13;
  double dVar14;
  double dVar15;
  undefined1 local_58 [4];
  int local_54;
  int local_50;
  undefined1 local_48 [4];
  int local_44;
  int local_40;
  
  iVar2 = *(int *)(param_3 + 8);
  lVar3 = *(long *)(param_3 + 0x28);
  lVar6 = (long)iVar2;
  uVar7 = tagNewIVector_vsdlss(param_1,lVar6,1,0);
  if (((DAT_00ec5dbc == 0) || (*(int *)(param_2 + 4) + *(int *)(param_2 + 8) < DAT_00ec5db8)) ||
     (iVar10 = iVar2, 0x1e < iVar2)) {
    iVar10 = DAT_00ec5db0;
    if (DAT_00ec5d90 < iVar2) {
      iVar10 = DAT_00ec5db4;
    }
    if (iVar2 < iVar10) {
      iVar10 = iVar2;
    }
  }
  dVar15 = (double)*(int *)(param_2 + 4);
  if (iVar10 < 1) {
    iVar10 = 1;
  }
  dVar14 = (double)*(int *)(param_2 + 8);
  local_44 = (int)(DAT_00ec5d98 * dVar15);
  if (local_44 < 1) {
    local_44 = 1;
  }
  local_40 = (int)(DAT_00ec5d98 * dVar14);
  if (local_40 < 1) {
    local_40 = 1;
  }
  local_54 = (int)(dVar15 * (_DAT_00ab67f8 / DAT_00ec5d98));
  if (local_54 < 1) {
    local_54 = 1;
  }
  iVar11 = (int)((_DAT_00ab67f8 / DAT_00ec5d98) * dVar14);
  local_50 = 1;
  if (0 < iVar11) {
    local_50 = iVar11;
  }
  if (iVar2 < 1) {
    iVar11 = 1;
  }
  else {
    lVar13 = 0;
    iVar11 = 0;
    do {
      iVar12 = *(int *)(lVar3 + 4 + lVar13);
      if (*(long *)(param_3 + 0x48) == 0) {
        lVar9 = 0;
      }
      else {
        lVar9 = *(long *)(*(long *)(param_3 + 0x48) + 8 + lVar13 * 2);
      }
      if (0 < iVar12) {
        piVar8 = (int *)(lVar9 + 4);
        piVar1 = (int *)(lVar9 + 8 + (ulong)(iVar12 - 1) * 4);
        do {
          while (lVar9 != 0) {
            iVar12 = *piVar8;
            piVar8 = piVar8 + 1;
            iVar11 = iVar11 + iVar12;
            if (piVar8 == piVar1) goto LAB_00a00b18;
          }
          piVar8 = piVar8 + 1;
          iVar11 = iVar11 + 1;
        } while (piVar8 != piVar1);
      }
LAB_00a00b18:
      lVar13 = lVar13 + 4;
    } while (lVar13 != (ulong)(iVar2 - 1) * 4 + 4);
    iVar11 = iVar11 + 1;
  }
  iVar12 = 1;
  do {
    while( true ) {
      iVar5 = rand();
      iVar5 = MLDGrowRegionBalanceRefine_vsdlss
                        ((iVar5 >> 3) % iVar2 + 1,param_3,param_2,local_48,local_58,param_4);
      if (iVar11 <= iVar5) break;
      iVar12 = iVar12 + 1;
      copyIVector_vsdlss(*(undefined8 *)(param_4 + 0xa0),lVar6,uVar7);
      iVar11 = iVar5;
      if (iVar10 < iVar12) goto LAB_00a00b8e;
    }
    iVar12 = iVar12 + 1;
  } while (iVar12 <= iVar10);
LAB_00a00b8e:
  uVar4 = *(undefined8 *)(param_4 + 0xa0);
  *(int *)(param_3 + 0x14) = iVar11;
  copyIVector_vsdlss(uVar7,lVar6,uVar4);
  return;
}


