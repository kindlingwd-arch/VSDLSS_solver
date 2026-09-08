/* VSDLSS function (decompiled by Ghidra) */
/* name: renameInsAndGetNodeToBlock_vsdlss  addr: 00a21ea0  size: 1b8 */
#include "vsdlss_ref.h"

/* signature: long renameInsAndGetNodeToBlock_vsdlss(long param_1,undefined8 *param_2); */

long renameInsAndGetNodeToBlock_vsdlss(long param_1,undefined8 *param_2)

{
  long lVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  undefined8 *puVar7;
  ulong uVar8;
  int *piVar9;
  ulong uVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  undefined8 local_48;
  
  piVar13 = (int *)*param_2;
  iVar3 = *piVar13;
  uVar10 = (ulong)iVar3;
  lVar4 = newIVector_vsdlss(uVar10,1,0);
  lVar1 = uVar10 * 8 + 8;
  lVar5 = casiMalloc_vsdlss(lVar1);
  if (lVar5 == 0) {
    nrerror_vsdlss(0xd,"indistnodesetsutils.c",0x87,lVar1,"polyNewVectorNoInit()");
  }
  uVar8 = 1;
  if (uVar10 != 0) {
    do {
      lVar6 = uVar8 * 8;
      lVar2 = uVar8 * 4;
      uVar8 = uVar8 + 1;
      *(undefined8 *)(lVar5 + (long)*(int *)(param_1 + lVar2) * 8) =
           *(undefined8 *)(*(long *)(piVar13 + 2) + lVar6);
    } while (uVar8 <= uVar10);
    lVar6 = 8;
    do {
      *(undefined8 *)(*(long *)(piVar13 + 2) + lVar6) = *(undefined8 *)(lVar5 + lVar6);
      lVar6 = lVar6 + 8;
    } while (lVar1 != lVar6);
  }
  if (lVar5 != 0) {
    casiFree_vsdlss(lVar5);
  }
  if (iVar3 < 1) {
    iVar11 = 0;
    local_48 = deleteIndistNodeSets_vsdlss(piVar13);
  }
  else {
    lVar1 = *(long *)(piVar13 + 2);
    iVar11 = 0;
    puVar7 = (undefined8 *)(lVar1 + 8);
    do {
      piVar9 = (int *)*puVar7;
      if (piVar9 != (int *)0x0) {
        iVar11 = iVar11 + 1;
        do {
          iVar12 = *piVar9;
          piVar9 = *(int **)(piVar9 + 2);
          *(int *)(lVar4 + (long)*(int *)(param_1 + (long)iVar12 * 4) * 4) = iVar11;
        } while (piVar9 != (int *)0x0);
      }
      puVar7 = puVar7 + 1;
    } while (puVar7 != (undefined8 *)(lVar1 + 0x10 + (ulong)(iVar3 - 1) * 8));
    local_48 = deleteIndistNodeSets_vsdlss(piVar13);
    if (1 < iVar3) {
      piVar13 = (int *)(lVar4 + 8);
      iVar12 = 2;
      do {
        if ((*piVar13 != piVar13[-1]) && (*piVar13 != piVar13[-1] + 1)) {
          nrerror_vsdlss(1,"indistnodesetsutils.c",0x9e);
        }
        iVar12 = iVar12 + 1;
        piVar13 = piVar13 + 1;
      } while (iVar12 <= iVar3);
    }
  }
  if (*(int *)(lVar4 + uVar10 * 4) != iVar11) {
    nrerror_vsdlss(1,"indistnodesetsutils.c",0xa3);
  }
  *param_2 = local_48;
  return lVar4;
}


