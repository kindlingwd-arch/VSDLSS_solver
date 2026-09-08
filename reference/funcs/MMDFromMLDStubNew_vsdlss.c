/* VSDLSS function (decompiled by Ghidra) */
/* name: MMDFromMLDStubNew_vsdlss  addr: 00a2c140  size: 44a */
#include "vsdlss_ref.h"

/* signature: void MMDFromMLDStubNew_vsdlss(long param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,undefined4 *param_5); */

void MMDFromMLDStubNew_vsdlss
               (long param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,
               undefined4 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  long *plVar4;
  int iVar5;
  bool bVar6;
  long *plVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  int *piVar14;
  long lVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  long lVar19;
  uint uVar20;
  long lVar21;
  long *local_b0;
  int local_a4;
  int local_78;
  undefined1 local_48 [24];
  
  iVar1 = *(int *)(param_1 + 8);
  lVar15 = *(long *)(param_5 + 8);
  uVar2 = *param_5;
  lVar9 = (long)iVar1;
  lVar10 = newIVector_vsdlss(lVar9,0,0);
  lVar3 = *(long *)(param_1 + 0x28);
  iVar5 = iVar1 >> 10;
  if (iVar1 < 0x400) {
    iVar5 = 1;
  }
  plVar4 = *(long **)(param_1 + 0x38);
  newCountingMarkNoHeader(local_48,iVar1);
  checkAtEndListOrderedAtEnd(iVar1,param_4,param_2,param_3);
  lVar11 = newIVector_vsdlss(lVar9,0,0);
  copyIVector_vsdlss(param_4,lVar9,lVar11);
  getInverseMap_vsdlss(lVar11,iVar1,param_4);
  if (lVar11 != 0) {
    casiFree_vsdlss(lVar11);
  }
  permuteMLDGraph(param_4,param_1);
  lVar11 = newIVector_vsdlss(lVar9,1,0);
  if (iVar1 < 1) {
    local_78 = 0;
  }
  else {
    lVar21 = 0;
    lVar19 = 0;
    local_a4 = 0;
    local_78 = 0;
    iVar17 = 1;
    iVar8 = 0;
    plVar7 = plVar4;
    do {
      local_b0 = plVar7 + 1;
      updateQalElimDup(param_1,iVar17,iVar8,lVar11,local_48);
      iVar18 = (int)lVar19;
      if ((*(int *)(lVar3 + 4 + lVar19 * 4) == 1) && (*(int *)(*local_b0 + 4) == iVar18)) {
        *(int *)(lVar11 + lVar19 * 4) = iVar17;
        *(undefined4 *)(lVar11 + 4 + lVar19 * 4) = 0;
        piVar14 = (int *)(lVar15 + (long)local_78 * 4);
        *piVar14 = *piVar14 + 1;
        *(int *)(lVar3 + 4 + lVar19 * 4) = *(int *)(lVar3 + lVar19 * 4) + -1;
        *local_b0 = *plVar7 + 4;
      }
      else {
        iVar8 = MLDFindReachArrRetSizeNew(iVar17,param_1,lVar10,lVar11,local_48);
        iVar16 = iVar8 + 1;
        lVar12 = lVar21;
        if (local_a4 < iVar16) {
          local_a4 = iVar8 + 2;
          if (iVar16 < iVar5) {
            local_a4 = iVar5;
          }
          lVar12 = newBlock_vsdlss(uVar2,(long)local_a4 << 2);
          local_a4 = local_a4 + -1;
        }
        local_a4 = local_a4 - iVar16;
        lVar21 = lVar12 + (long)iVar16 * 4;
        *(int *)(lVar12 + 4) = iVar17;
        uVar13 = 1;
        if ((long)iVar8 != 0) {
          do {
            *(undefined4 *)(lVar12 + 4 + uVar13 * 4) = *(undefined4 *)(lVar10 + uVar13 * 4);
            uVar13 = uVar13 + 1;
          } while (uVar13 <= (ulong)(long)iVar8);
        }
        if (iVar8 < 0x41) {
          uVar20 = iVar8 - 2;
          if (0 < iVar8) {
            for (; uVar20 != 0xffffffff; uVar20 = uVar20 - 1) {
              piVar14 = (int *)(lVar12 + 8);
              bVar6 = true;
              do {
                iVar16 = *piVar14;
                if (piVar14[1] < iVar16) {
                  *piVar14 = piVar14[1];
                  piVar14[1] = iVar16;
                  bVar6 = false;
                }
                piVar14 = piVar14 + 1;
              } while (piVar14 != (int *)(lVar12 + 0xc + (ulong)uVar20 * 4));
              if (bVar6) break;
            }
          }
          if (1 < iVar8) goto LAB_00a2c3ae;
        }
        else {
          casiRadixSortBase8(iVar8,lVar10,lVar12 + 4);
LAB_00a2c3ae:
          piVar14 = (int *)(lVar12 + 8);
          iVar16 = 2;
          do {
            while (piVar14[1] <= *piVar14) {
              iVar16 = iVar16 + 1;
              piVar14 = piVar14 + 1;
              nrerror_vsdlss(1,"mldmmd.c",0x9c6);
              if (iVar8 < iVar16) goto LAB_00a2c3fa;
            }
            iVar16 = iVar16 + 1;
            piVar14 = piVar14 + 1;
          } while (iVar16 <= iVar8);
        }
LAB_00a2c3fa:
        local_78 = local_78 + 1;
        *(undefined4 *)(lVar15 + (long)local_78 * 4) = 1;
        *(int *)(lVar3 + 4 + lVar19 * 4) = iVar8;
        *local_b0 = lVar12 + 4;
        iVar8 = iVar17;
      }
      lVar19 = lVar19 + 1;
      iVar17 = iVar18 + 2;
      plVar7 = local_b0;
    } while (iVar17 <= iVar1);
  }
  if (lVar11 != 0) {
    casiFree_vsdlss();
  }
  deleteCountingMarkNoHeader(local_48);
  if (lVar10 != 0) {
    casiFree_vsdlss(lVar10);
  }
  param_5[5] = local_78;
  param_5[7] = local_78;
  param_5[6] = 1;
  lVar15 = tagNewIVector_vsdlss(uVar2,lVar9,0,0);
  *(long *)(param_5 + 10) = lVar15;
  lVar9 = tagNewIPVector_vsdlss(uVar2,lVar9,1,0);
  *(long *)(param_5 + 0xc) = lVar9;
  if (0 < iVar1) {
    lVar10 = 0;
    do {
      *(int *)(lVar15 + 4 + lVar10) = *(int *)(lVar3 + 4 + lVar10) + 1;
      lVar11 = *(long *)((long)plVar4 + lVar10 * 2 + 8);
      *(undefined4 *)(lVar3 + 4 + lVar10) = 0;
      *(long *)(lVar9 + 8 + lVar10 * 2) = lVar11 + -4;
      *(undefined8 *)((long)plVar4 + lVar10 * 2 + 8) = 0;
      lVar10 = lVar10 + 4;
    } while (lVar10 != (ulong)(iVar1 - 1) * 4 + 4);
  }
  return;
}


