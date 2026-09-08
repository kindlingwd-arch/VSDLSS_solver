/* VSDLSS function (decompiled by Ghidra) */
/* name: arrayBasedBucketByDigit_vsdlss  addr: 00a1e2b0  size: 1a9 */
#include "vsdlss_ref.h"

/* signature: undefined4 * arrayBasedBucketByDigit_vsdlss(long param_1,int param_2,int param_3,long param_4); */

undefined4 * arrayBasedBucketByDigit_vsdlss(long param_1,int param_2,int param_3,long param_4)

{
  int *piVar1;
  undefined4 uVar2;
  uint uVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  undefined4 *puVar7;
  int *piVar8;
  long lVar9;
  int *piVar10;
  long lVar11;
  long lVar12;
  int iVar13;
  int local_50;
  
  puVar7 = (undefined4 *)newArrBucketArr_vsdlss(param_3);
  lVar4 = *(long *)(puVar7 + 4);
  lVar5 = *(long *)(puVar7 + 6);
  lVar11 = *(long *)(puVar7 + 2);
  uVar2 = *puVar7;
  if (0 < param_2) {
    piVar8 = (int *)(param_4 + 4);
    do {
      iVar13 = *piVar8;
      piVar8 = piVar8 + 1;
      piVar1 = (int *)(lVar4 + (long)*(int *)(param_1 + (long)iVar13 * 4) * 4);
      *piVar1 = *piVar1 + 1;
    } while (piVar8 != (int *)(param_4 + 8 + (ulong)(param_2 - 1) * 4));
  }
  if (param_3 < 0x400) {
    local_50 = 1;
    if (param_3 < 1) goto LAB_00a1e3a0;
  }
  else {
    local_50 = param_3 >> 10;
  }
  iVar13 = 0;
  lVar9 = 0;
  lVar12 = 0;
  do {
    uVar3 = *(uint *)(lVar4 + 4 + lVar12 * 4);
    if (iVar13 <= (int)uVar3) {
      iVar13 = uVar3 + 1;
      if ((int)uVar3 < local_50) {
        iVar13 = local_50;
      }
      lVar9 = newBlock_vsdlss(uVar2,(long)iVar13 << 2);
    }
    *(long *)(lVar11 + 8 + lVar12 * 8) = lVar9;
    lVar9 = lVar9 + 4 + (long)(int)uVar3 * 4;
    iVar6 = (int)lVar12;
    iVar13 = iVar13 + ~uVar3;
    lVar12 = lVar12 + 1;
  } while (iVar6 + 2 <= param_3);
LAB_00a1e3a0:
  if (0 < param_2) {
    piVar8 = (int *)(param_4 + 4);
    do {
      piVar10 = piVar8 + 1;
      lVar9 = (long)*(int *)(param_1 + (long)*piVar8 * 4);
      piVar1 = (int *)(lVar5 + lVar9 * 4);
      lVar9 = *(long *)(lVar11 + lVar9 * 8);
      iVar13 = *piVar1 + 1;
      *piVar1 = iVar13;
      *(int *)(lVar9 + (long)iVar13 * 4) = *piVar8;
      piVar8 = piVar10;
    } while (piVar10 != (int *)(param_4 + 8 + (ulong)(param_2 - 1) * 4));
  }
  if (0 < param_3) {
    if (*(int *)(lVar5 + 4) == *(int *)(lVar4 + 4)) {
      lVar11 = 0;
      do {
        if (param_3 < (int)lVar11 + 2) {
          return puVar7;
        }
        lVar9 = lVar11 * 4;
        lVar11 = lVar11 + 1;
      } while (*(int *)(lVar5 + 8 + lVar9) == *(int *)(lVar4 + 4 + lVar11 * 4));
    }
    nrerror_vsdlss(3,"arrbuckarrutils.c",0x7f);
  }
  return puVar7;
}


