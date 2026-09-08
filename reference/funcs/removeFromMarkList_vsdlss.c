/* VSDLSS function (decompiled by Ghidra) */
/* name: removeFromMarkList_vsdlss  addr: 00a1ee60  size: ea */
#include "vsdlss_ref.h"

/* signature: int removeFromMarkList_vsdlss(uint *param_1); */

int removeFromMarkList_vsdlss(uint *param_1)

{
  uint *puVar1;
  uint uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  
  uVar7 = param_1[5];
  uVar6 = param_1[4];
  if (uVar7 == uVar6) {
    nrerror_vsdlss(1,"bpqutils.c",0xd2);
    uVar6 = param_1[4];
    uVar7 = param_1[5];
  }
  uVar2 = *param_1;
  iVar8 = -uVar7;
  lVar3 = *(long *)(param_1 + 8);
  lVar4 = *(long *)(param_1 + 10);
  if (((int)uVar2 < iVar8) || (iVar8 < 1)) {
    nrerror_vsdlss(1,"bpqutils.c",0xdd);
  }
  puVar1 = (uint *)(lVar4 + (long)iVar8 * 4);
  uVar7 = *puVar1;
  param_1[5] = uVar7;
  if (uVar7 != uVar6) {
    iVar5 = -uVar7;
    if (((int)uVar2 < iVar5) || (iVar5 < 1)) {
      nrerror_vsdlss(1,"bpqutils.c",0xe5);
    }
    *(uint *)(lVar3 + (long)iVar5 * 4) = ~uVar2;
  }
  param_1[3] = param_1[3] - 1;
  *(uint *)(lVar3 + (long)iVar8 * 4) = uVar6;
  *puVar1 = uVar6;
  return iVar8;
}


