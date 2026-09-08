/* VSDLSS function (decompiled by Ghidra) */
/* name: permuteGraph_vsdlss  addr: 009f4e60  size: 123 */
#include "vsdlss_ref.h"

/* signature: void permuteGraph_vsdlss(long *param_1,long param_2); */

void permuteGraph_vsdlss(long *param_1,long param_2)

{
  long lVar1;
  long lVar2;
  int iVar3;
  int *piVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  undefined8 uVar8;
  ulong uVar9;
  ulong uVar10;
  
  piVar4 = (int *)*param_1;
  iVar3 = *piVar4;
  lVar6 = newGraph_vsdlss(iVar3);
  if (*(long *)(piVar4 + 2) == 0) {
    lVar7 = newCEdgeVector_vsdlss(iVar3,0,0);
    *(long *)(lVar6 + 0x18) = lVar7;
    if (iVar3 == 0) goto LAB_009f4f2d;
  }
  else {
    uVar10 = (ulong)iVar3;
    lVar7 = newIVector_vsdlss(uVar10,0,0);
    *(long *)(lVar6 + 8) = lVar7;
    if (uVar10 == 0) {
      uVar8 = newCEdgeVector_vsdlss(iVar3,0,0);
      *(undefined8 *)(lVar6 + 0x18) = uVar8;
      goto LAB_009f4f2d;
    }
    lVar5 = *(long *)(piVar4 + 2);
    uVar9 = 1;
    do {
      lVar1 = uVar9 * 4;
      lVar2 = uVar9 * 4;
      uVar9 = uVar9 + 1;
      *(undefined4 *)(lVar7 + (long)*(int *)(param_2 + lVar1) * 4) = *(undefined4 *)(lVar5 + lVar2);
    } while (uVar9 <= uVar10);
    lVar7 = newCEdgeVector_vsdlss(iVar3,0,0);
    *(long *)(lVar6 + 0x18) = lVar7;
  }
  uVar10 = 1;
  while( true ) {
    lVar5 = uVar10 * 8;
    lVar1 = uVar10 * 4;
    uVar10 = uVar10 + 1;
    *(undefined8 *)(lVar7 + (long)*(int *)(param_2 + lVar1) * 8) =
         *(undefined8 *)(*(long *)(piVar4 + 6) + lVar5);
    if ((ulong)(long)iVar3 < uVar10) break;
    lVar7 = *(long *)(lVar6 + 0x18);
  }
  uVar10 = 1;
  do {
    *(undefined8 *)(*(long *)(piVar4 + 6) + uVar10 * 8) = 0;
    uVar10 = uVar10 + 1;
  } while (uVar10 <= (ulong)(long)iVar3);
LAB_009f4f2d:
  deleteGraph_vsdlss(piVar4);
  renameNodesInAdjListOfGraph_vsdlss(lVar6,param_2);
  *param_1 = lVar6;
  return;
}


