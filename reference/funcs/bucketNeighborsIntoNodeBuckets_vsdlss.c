/* VSDLSS function (decompiled by Ghidra) */
/* name: bucketNeighborsIntoNodeBuckets_vsdlss  addr: 00a1e470  size: 1e1 */
#include "vsdlss_ref.h"

/* signature: undefined4 * bucketNeighborsIntoNodeBuckets_vsdlss(int *param_1); */

undefined4 * bucketNeighborsIntoNodeBuckets_vsdlss(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  undefined4 *puVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  int iVar16;
  int local_48;
  
  iVar2 = param_1[1];
  iVar3 = *param_1;
  lVar14 = *(long *)(param_1 + 6);
  lVar8 = *(long *)(param_1 + 10);
  puVar12 = (undefined4 *)newArrBucketArr_vsdlss(iVar2);
  lVar9 = *(long *)(puVar12 + 4);
  lVar10 = *(long *)(puVar12 + 6);
  lVar11 = *(long *)(puVar12 + 2);
  uVar4 = *puVar12;
  if (0 < iVar3) {
    lVar13 = 0;
    do {
      piVar1 = (int *)(lVar14 + 4 + lVar13);
      iVar16 = *(int *)(lVar8 + 4 + lVar13);
      lVar13 = lVar13 + 4;
      piVar1 = (int *)(lVar9 + (long)*piVar1 * 4);
      *piVar1 = *piVar1 + 1;
      piVar1 = (int *)(lVar9 + (long)iVar16 * 4);
      *piVar1 = *piVar1 + 1;
    } while (lVar13 != (ulong)(iVar3 - 1) * 4 + 4);
  }
  if (iVar2 < 0x400) {
    local_48 = 1;
    if (iVar2 < 1) goto LAB_00a1e587;
  }
  else {
    local_48 = iVar2 >> 10;
  }
  iVar16 = 0;
  lVar13 = 0;
  lVar15 = 0;
  do {
    uVar5 = *(uint *)(lVar9 + 4 + lVar15 * 4);
    if (iVar16 <= (int)uVar5) {
      iVar16 = uVar5 + 1;
      if ((int)uVar5 < local_48) {
        iVar16 = local_48;
      }
      lVar13 = newBlock_vsdlss(uVar4,(long)iVar16 << 2);
    }
    *(long *)(lVar11 + 8 + lVar15 * 8) = lVar13;
    lVar13 = lVar13 + 4 + (long)(int)uVar5 * 4;
    iVar6 = (int)lVar15;
    iVar16 = iVar16 + ~uVar5;
    lVar15 = lVar15 + 1;
  } while (iVar6 + 2 <= iVar2);
LAB_00a1e587:
  if (0 < iVar3) {
    lVar13 = 0;
    do {
      iVar6 = *(int *)(lVar14 + 4 + lVar13);
      iVar7 = *(int *)(lVar8 + 4 + lVar13);
      lVar13 = lVar13 + 4;
      piVar1 = (int *)(lVar10 + (long)iVar6 * 4);
      lVar15 = *(long *)(lVar11 + (long)iVar6 * 8);
      iVar16 = *piVar1 + 1;
      *piVar1 = iVar16;
      *(int *)(lVar15 + (long)iVar16 * 4) = iVar7;
      piVar1 = (int *)(lVar10 + (long)iVar7 * 4);
      lVar15 = *(long *)(lVar11 + (long)iVar7 * 8);
      iVar16 = *piVar1 + 1;
      *piVar1 = iVar16;
      *(int *)(lVar15 + (long)iVar16 * 4) = iVar6;
    } while (lVar13 != (ulong)(iVar3 - 1) * 4 + 4);
  }
  if (0 < iVar2) {
    if (*(int *)(lVar10 + 4) == *(int *)(lVar9 + 4)) {
      lVar14 = 0;
      do {
        if (iVar2 < (int)lVar14 + 2) {
          return puVar12;
        }
        lVar8 = lVar14 * 4;
        lVar14 = lVar14 + 1;
      } while (*(int *)(lVar10 + 8 + lVar8) == *(int *)(lVar9 + 4 + lVar14 * 4));
    }
    nrerror_vsdlss(3,"arrbuckarrutils.c",0xce);
  }
  return puVar12;
}


