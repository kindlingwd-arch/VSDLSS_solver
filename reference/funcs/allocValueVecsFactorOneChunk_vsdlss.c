/* VSDLSS function (decompiled by Ghidra) */
/* name: allocValueVecsFactorOneChunk_vsdlss  addr: 00a15e00  size: 100 */
#include "vsdlss_ref.h"

/* signature: void allocValueVecsFactorOneChunk_vsdlss(undefined4 *param_1); */

void allocValueVecsFactorOneChunk_vsdlss(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  ulong uVar9;
  int iVar10;
  long lVar11;
  int local_3c;
  
  iVar1 = param_1[2];
  uVar2 = *param_1;
  lVar4 = *(long *)(param_1 + 10);
  if (iVar1 < 0x400) {
    lVar5 = tagNewFPVector_vsdlss(uVar2,(long)iVar1,1);
    *(long *)(param_1 + 0xe) = lVar5;
    local_3c = 1;
    if (iVar1 < 1) {
      return;
    }
  }
  else {
    local_3c = iVar1 >> 10;
    lVar5 = tagNewFPVector_vsdlss(uVar2,(long)iVar1,1);
    *(long *)(param_1 + 0xe) = lVar5;
  }
  iVar10 = 0;
  lVar7 = 0;
  lVar11 = 0;
  do {
    iVar3 = *(int *)(lVar4 + 4 + lVar11 * 4);
    lVar6 = lVar7;
    if (iVar10 < iVar3) {
      iVar10 = iVar3 + 1;
      if (iVar3 < local_3c) {
        iVar10 = local_3c;
      }
      lVar6 = newBlock_vsdlss(uVar2,(long)iVar10 << 3);
      iVar10 = iVar10 + -1;
    }
    uVar9 = (ulong)iVar3;
    iVar10 = iVar10 - iVar3;
    uVar8 = 1;
    lVar7 = lVar6 + uVar9 * 8;
    if (uVar9 != 0) {
      do {
        *(undefined8 *)(lVar6 + uVar8 * 8) = 0;
        uVar8 = uVar8 + 1;
      } while (uVar8 <= uVar9);
    }
    *(long *)(lVar5 + 8 + lVar11 * 8) = lVar6;
    iVar3 = (int)lVar11;
    lVar11 = lVar11 + 1;
  } while (iVar3 + 2 <= iVar1);
  return;
}


