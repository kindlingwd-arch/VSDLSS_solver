/* VSDLSS function (decompiled by Ghidra) */
/* name: computeSplitBlockForChunks_vsdlss  addr: 009f24e0  size: 1ce */
#include "vsdlss_ref.h"

/* signature: void computeSplitBlockForChunks_vsdlss(long param_1,long param_2); */

void computeSplitBlockForChunks_vsdlss(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long lVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  
  dVar7 = DAT_00ab7228;
  iVar1 = *(int *)(param_2 + 8);
  if (0 < iVar1) {
    lVar13 = 4;
    lVar5 = *(long *)(param_2 + 0x38);
    lVar6 = *(long *)(param_2 + 0x48);
    do {
      iVar12 = *(int *)(lVar5 + lVar13);
      iVar2 = *(int *)(lVar6 + lVar13);
      if (iVar12 <= iVar2) {
        lVar14 = (long)iVar12 * 4;
        lVar9 = (ulong)(uint)(iVar2 - iVar12) * 4 + 4;
        lVar8 = 0;
        dVar15 = 0.0;
        do {
          while( true ) {
            iVar3 = *(int *)(*(long *)(param_1 + 0x18) + lVar14 + lVar8);
            iVar4 = *(int *)(*(long *)(param_1 + 0x10) + lVar14 + lVar8);
            lVar11 = (long)iVar3 << 2;
            if (lVar11 < 0) {
              dVar16 = (double)(((long)iVar3 & 0x3fffffffffffffffU) << 1);
              dVar16 = dVar16 + dVar16;
            }
            else {
              dVar16 = (double)lVar11;
            }
            uVar10 = (ulong)(iVar3 * iVar4 - ((iVar4 + -1) * iVar4) / 2);
            lVar11 = uVar10 << 3;
            if (lVar11 < 0) break;
            lVar8 = lVar8 + 4;
            dVar15 = (double)lVar11 + dVar16 + dVar15;
            if (lVar8 == lVar9) goto LAB_009f2601;
          }
          lVar8 = lVar8 + 4;
          dVar18 = (double)((uVar10 & 0x1fffffffffffffff) << 2);
          dVar15 = dVar18 + dVar18 + dVar16 + dVar15;
        } while (lVar8 != lVar9);
LAB_009f2601:
        dVar16 = 0.0;
        do {
          iVar3 = *(int *)(*(long *)(param_1 + 0x18) + lVar14);
          iVar4 = *(int *)(*(long *)(param_1 + 0x10) + lVar14);
          lVar9 = (long)iVar3 << 2;
          if (lVar9 < 0) {
            dVar18 = (double)(((long)iVar3 & 0x3fffffffffffffffU) << 1);
            dVar18 = dVar18 + dVar18;
          }
          else {
            dVar18 = (double)lVar9;
          }
          uVar10 = (ulong)(iVar3 * iVar4 - ((iVar4 + -1) * iVar4) / 2);
          lVar9 = uVar10 << 3;
          if (lVar9 < 0) {
            dVar17 = (double)((uVar10 & 0x1fffffffffffffff) << 2);
            dVar17 = dVar17 + dVar17;
          }
          else {
            dVar17 = (double)lVar9;
          }
          dVar16 = dVar16 + dVar18 + dVar17;
          if (dVar15 * dVar7 <= dVar16) {
            *(int *)(*(long *)(param_2 + 0x40) + lVar13) = iVar12;
            *(undefined4 *)(*(long *)(param_2 + 0x58) + lVar13) =
                 *(undefined4 *)(*(long *)(param_1 + 8) + lVar14);
            break;
          }
          iVar12 = iVar12 + 1;
          lVar14 = lVar14 + 4;
        } while (iVar12 <= iVar2);
      }
      lVar13 = lVar13 + 4;
    } while (lVar13 != (ulong)(iVar1 - 1) * 4 + 8);
  }
  return;
}


