/* VSDLSS function (decompiled by Ghidra) */
/* name: transferGraphToFactorChunkSort_vsdlss  addr: 00a15b70  size: 28f */
#include "vsdlss_ref.h"

/* signature: void transferGraphToFactorChunkSort_vsdlss(long param_1,long param_2,undefined4 *param_3); */

void transferGraphToFactorChunkSort_vsdlss(long param_1,long param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  bool bVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  ulong uVar12;
  int *piVar13;
  int iVar14;
  uint uVar15;
  int *piVar16;
  int local_68;
  int local_60;
  long local_48;
  
  iVar1 = param_3[5];
  iVar8 = param_3[2];
  uVar2 = *param_3;
  lVar5 = *(long *)(param_2 + 0x28);
  lVar6 = *(long *)(param_2 + 8);
  piVar16 = *(int **)(param_3 + 8);
  lVar9 = tagNewIVector_vsdlss(uVar2,(long)iVar8,0,0);
  *(long *)(param_3 + 10) = lVar9;
  lVar10 = tagNewIPVector_vsdlss(uVar2,(long)iVar8,1,0);
  *(long *)(param_3 + 0xc) = lVar10;
  if (0 < iVar8) {
    lVar11 = 0;
    do {
      *(int *)(lVar9 + (long)*(int *)(param_1 + 4 + lVar11) * 4) = *(int *)(lVar6 + 4 + lVar11) + 1;
      *(undefined4 *)(lVar6 + 4 + lVar11) = 0;
      *(long *)(lVar10 + (long)*(int *)(param_1 + 4 + lVar11) * 8) =
           *(long *)(lVar5 + 8 + lVar11 * 2) + -4;
      *(undefined8 *)(lVar5 + 8 + lVar11 * 2) = 0;
      lVar11 = lVar11 + 4;
    } while (lVar11 != (ulong)(iVar8 - 1) * 4 + 4);
  }
  iVar8 = getMaxIVector_vsdlss(lVar9,iVar8);
  if (iVar8 < 0x41) {
    local_48 = 0;
  }
  else {
    local_48 = newIVector_vsdlss((long)iVar8,0,0);
  }
  local_60 = 1;
  local_68 = 1;
  if (0 < iVar1) {
    do {
      piVar16 = piVar16 + 1;
      iVar3 = *piVar16;
      iVar4 = *(int *)(lVar9 + (long)local_68 * 4);
      lVar5 = *(long *)(lVar10 + (long)local_68 * 8);
      uVar12 = 1;
      if ((long)iVar4 != 0) {
        do {
          *(undefined4 *)(lVar5 + uVar12 * 4) =
               *(undefined4 *)(param_1 + (long)*(int *)(lVar5 + uVar12 * 4) * 4);
          uVar12 = uVar12 + 1;
        } while (uVar12 <= (ulong)(long)iVar4);
      }
      iVar14 = iVar4 - iVar3;
      lVar5 = lVar5 + (long)iVar3 * 4;
      if (iVar14 < 0x41) {
        uVar15 = iVar14 - 2;
        if (0 < iVar14) {
          for (; uVar15 != 0xffffffff; uVar15 = uVar15 - 1) {
            piVar13 = (int *)(lVar5 + 4);
            bVar7 = true;
            do {
              iVar14 = *piVar13;
              if (piVar13[1] < iVar14) {
                *piVar13 = piVar13[1];
                piVar13[1] = iVar14;
                bVar7 = false;
              }
              piVar13 = piVar13 + 1;
            } while (piVar13 != (int *)(lVar5 + 8 + (ulong)uVar15 * 4));
            if (bVar7) break;
          }
        }
      }
      else {
        casiRadixSortBase8(iVar14,local_48,lVar5);
      }
      iVar14 = 2;
      piVar13 = (int *)(lVar5 + (long)iVar3 * -4 + 4);
      if (1 < iVar4) {
        do {
          while (piVar13[1] <= *piVar13) {
            iVar14 = iVar14 + 1;
            piVar13 = piVar13 + 1;
            nrerror_vsdlss(1,"vsdlss1memutils.c",0x1e9);
            if (iVar4 < iVar14) goto LAB_00a15d89;
          }
          iVar14 = iVar14 + 1;
          piVar13 = piVar13 + 1;
        } while (iVar14 <= iVar4);
      }
LAB_00a15d89:
      local_60 = local_60 + 1;
      local_68 = local_68 + iVar3;
    } while (local_60 <= iVar1);
  }
  if ((0x40 < iVar8) && (local_48 != 0)) {
    casiFree_vsdlss();
    return;
  }
  return;
}


