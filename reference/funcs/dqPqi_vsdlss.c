/* VSDLSS function (decompiled by Ghidra) */
/* name: dqPqi_vsdlss  addr: 00a0d080  size: 161 */
#include "vsdlss_ref.h"

/* signature: int dqPqi_vsdlss(long param_1,undefined4 *param_2); */

int dqPqi_vsdlss(long param_1,undefined4 *param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  long lVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  
  if (*(int *)(param_1 + 8) < 1) {
    nrerror_vsdlss(5,"priorityqutils_i.c",0x212);
  }
  iVar3 = *(int *)(*(long *)(param_1 + 0x18) + 4);
  *param_2 = *(undefined4 *)(*(long *)(param_1 + 0x20) + (long)iVar3 * 4);
  pqiSwap_vsdlss(1,*(undefined4 *)(param_1 + 8),param_1);
  iVar14 = *(int *)(param_1 + 8) + -1;
  *(int *)(param_1 + 8) = iVar14;
  if (iVar14 < 2) {
    lVar13 = *(long *)(param_1 + 0x10);
  }
  else {
    lVar8 = *(long *)(param_1 + 0x20);
    lVar9 = *(long *)(param_1 + 0x18);
    lVar13 = *(long *)(param_1 + 0x10);
    iVar11 = 2;
    iVar15 = 1;
    do {
      iVar10 = iVar11;
      if (iVar11 < iVar14) {
        piVar1 = (int *)(lVar9 + (long)iVar11 * 4);
        piVar2 = (int *)(lVar9 + 4 + (long)iVar11 * 4);
        iVar4 = *piVar1;
        iVar5 = *piVar2;
        iVar6 = *(int *)(lVar8 + (long)iVar4 * 4);
        iVar7 = *(int *)(lVar8 + (long)iVar5 * 4);
        piVar12 = piVar1;
        iVar14 = iVar4;
        iVar16 = iVar6;
        if (((iVar7 <= iVar6) &&
            (piVar12 = piVar2, iVar14 = iVar5, iVar16 = iVar7, iVar10 = iVar11 + 1, iVar6 <= iVar7))
           && (piVar12 = piVar1, iVar14 = iVar4, iVar16 = iVar6, iVar10 = iVar11, iVar5 < iVar4)) {
          piVar12 = piVar2;
          iVar14 = iVar5;
          iVar16 = iVar7;
          iVar10 = iVar11 + 1;
        }
      }
      else {
        piVar12 = (int *)(lVar9 + (long)iVar11 * 4);
        iVar14 = *piVar12;
        iVar16 = *(int *)(lVar8 + (long)iVar14 * 4);
      }
      piVar1 = (int *)(lVar9 + (long)iVar15 * 4);
      iVar11 = *piVar1;
      iVar4 = *(int *)(lVar8 + (long)iVar11 * 4);
      if (((iVar4 <= iVar16) && ((iVar4 < iVar16 || (iVar11 < iVar14)))) &&
         (piVar12 = piVar1, iVar14 = iVar11, iVar10 != iVar15)) break;
      *piVar1 = iVar14;
      *piVar12 = iVar11;
      *(int *)(lVar13 + (long)*piVar1 * 4) = iVar15;
      iVar11 = iVar10 * 2;
      *(int *)(lVar13 + (long)*piVar12 * 4) = iVar10;
      iVar14 = *(int *)(param_1 + 8);
      iVar15 = iVar10;
    } while (iVar11 <= iVar14);
  }
  *(undefined4 *)(lVar13 + (long)iVar3 * 4) = 0xffffffff;
  return iVar3;
}


