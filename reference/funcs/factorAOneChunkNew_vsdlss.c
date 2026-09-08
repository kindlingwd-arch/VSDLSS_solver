/* VSDLSS function (decompiled by Ghidra) */
/* name: factorAOneChunkNew_vsdlss  addr: 00a1abc0  size: 282 */
#include "vsdlss_ref.h"

/* signature: void factorAOneChunkNew_vsdlss(undefined8 param_1,int param_2,undefined4 param_3,long param_4); */

void factorAOneChunkNew_vsdlss(undefined8 param_1,int param_2,undefined4 param_3,long param_4)

{
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  long lVar14;
  
  iVar11 = *(int *)(param_4 + 0x14);
  lVar1 = *(long *)(param_4 + 0x20);
  iVar3 = getMaxIVector_vsdlss(*(undefined8 *)(param_4 + 0x28),*(undefined4 *)(param_4 + 8));
  iVar4 = getMaxSupNodeNZ_vsdlss(param_4);
  lVar5 = newIVector_vsdlss((long)iVar3,0,0);
  lVar6 = newVector_vsdlss(0,0x4011,0);
  lVar7 = newFVector_vsdlss(0,(long)(iVar4 + 0x10),0);
  lVar8 = newFVector_vsdlss(0,(long)(iVar4 + 0x10),0);
  uVar2 = casiParSolverF;
  lVar10 = (ulong)(0x10 - ((uint)lVar6 & 0xf)) + lVar6;
  lVar9 = (ulong)(0x10 - ((uint)lVar7 & 0xf)) + lVar7;
  lVar14 = (ulong)(0x10 - ((uint)lVar8 & 0xf)) + lVar8;
  if (param_2 != 0) {
    iVar3 = getNBlocksPartFact_vsdlss(iVar11,lVar1,param_3);
    iVar11 = iVar11 - iVar3;
  }
  piVar13 = (int *)(lVar1 + 4);
  iVar4 = 1;
  iVar3 = 1;
  if (0 < iVar11) {
    do {
      while (iVar12 = *piVar13, iVar12 != 2) {
        if (iVar12 == 3) {
          sub3RowContribFromRest_vsdlss(iVar3,param_4);
          iVar3 = iVar3 + 3;
        }
        else if (iVar12 == 1) {
          sub1RowContribFromRest_vsdlss(iVar3,param_4);
          iVar3 = iVar3 + 1;
        }
        else {
          iVar12 = iVar12 + iVar3;
          factorWithinBlock_r(iVar3,iVar12 + -1,param_1,param_4,uVar2,lVar5,lVar10,lVar9,lVar14);
          subBlockContribFromRowRange_vsdlss
                    (iVar3,iVar12 + -1,iVar12,*(undefined4 *)(param_4 + 8),param_1,param_4,uVar2,
                     lVar5,lVar10,lVar9,lVar14);
          iVar3 = iVar12;
        }
        iVar4 = iVar4 + 1;
        piVar13 = piVar13 + 1;
        if (iVar11 < iVar4) goto LAB_00a1adae;
      }
      iVar4 = iVar4 + 1;
      sub2RowContribFromRest_vsdlss(iVar3,param_4);
      iVar3 = iVar3 + 2;
      piVar13 = piVar13 + 1;
    } while (iVar4 <= iVar11);
  }
LAB_00a1adae:
  if (lVar5 != 0) {
    casiFree_vsdlss(lVar5);
  }
  if (lVar6 != 0) {
    casiFree_vsdlss();
  }
  if (lVar7 != 0) {
    casiFree_vsdlss();
  }
  if (lVar8 == 0) {
    return;
  }
  casiFree_vsdlss();
  return;
}


