/* VSDLSS function (decompiled by Ghidra) */
/* name: transferMLDGraphToFactorChunkSort_vsdlss  addr: 00a06420  size: 278 */
#include "vsdlss_ref.h"

/* signature: void transferMLDGraphToFactorChunkSort_vsdlss(long param_1,long param_2,undefined4 *param_3); */

void transferMLDGraphToFactorChunkSort_vsdlss(long param_1,long param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
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
  int iVar17;
  int local_68;
  long local_48;
  
  iVar8 = param_3[2];
  uVar1 = *param_3;
  iVar2 = param_3[5];
  lVar5 = *(long *)(param_2 + 0x38);
  lVar6 = *(long *)(param_2 + 0x28);
  piVar16 = *(int **)(param_3 + 8);
  lVar9 = tagNewIVector_vsdlss(uVar1,(long)iVar8,0,0);
  *(long *)(param_3 + 10) = lVar9;
  lVar10 = tagNewIPVector_vsdlss(uVar1,(long)iVar8,1,0);
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
  iVar14 = 1;
  local_68 = 1;
  if (0 < iVar2) {
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
      if (iVar4 < 0x41) {
        uVar15 = iVar4 - 2;
        if (0 < iVar4) {
          for (; uVar15 != 0xffffffff; uVar15 = uVar15 - 1) {
            piVar13 = (int *)(lVar5 + 4);
            bVar7 = true;
            do {
              iVar17 = *piVar13;
              if (piVar13[1] < iVar17) {
                *piVar13 = piVar13[1];
                piVar13[1] = iVar17;
                bVar7 = false;
              }
              piVar13 = piVar13 + 1;
            } while (piVar13 != (int *)(lVar5 + 8 + (ulong)uVar15 * 4));
            if (bVar7) break;
          }
        }
        if (1 < iVar4) goto LAB_00a065dd;
      }
      else {
        casiRadixSortBase8(iVar4,local_48,lVar5);
LAB_00a065dd:
        piVar13 = (int *)(lVar5 + 4);
        iVar17 = 2;
        do {
          while (*piVar13 < piVar13[1]) {
            iVar17 = iVar17 + 1;
            piVar13 = piVar13 + 1;
            if (iVar4 < iVar17) goto LAB_00a06627;
          }
          iVar17 = iVar17 + 1;
          piVar13 = piVar13 + 1;
          nrerror_vsdlss(1,"mldorder.c",0x1694);
        } while (iVar17 <= iVar4);
      }
LAB_00a06627:
      iVar14 = iVar14 + 1;
      local_68 = local_68 + iVar3;
    } while (iVar14 <= iVar2);
  }
  if ((0x40 < iVar8) && (local_48 != 0)) {
    casiFree_vsdlss();
    return;
  }
  return;
}


