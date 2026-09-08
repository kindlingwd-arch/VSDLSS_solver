/* VSDLSS function (decompiled by Ghidra) */
/* name: setCompGraphPointers_vsdlss  addr: 009fd7a0  size: 193 */
#include "vsdlss_ref.h"

/* signature: long setCompGraphPointers_vsdlss(long param_1,long param_2,int param_3,int param_4,int param_5,undefined4 *param_6,int *param_7); */

long setCompGraphPointers_vsdlss
               (long param_1,long param_2,int param_3,int param_4,int param_5,undefined4 *param_6,
               int *param_7)

{
  long lVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  uint uVar7;
  int iVar8;
  long lVar9;
  int *piVar10;
  long lVar11;
  
  lVar1 = (long)param_3 * 4 + -4;
  param_6[1] = 0;
  uVar3 = *(undefined4 *)(param_1 + 4);
  *(undefined8 *)(param_6 + 10) = 0;
  *(undefined8 *)(param_6 + 0x18) = 0;
  *(undefined8 *)(param_6 + 0x16) = 0;
  lVar11 = *(long *)(param_1 + 0x58);
  param_6[1] = uVar3;
  lVar5 = *(long *)(param_1 + 0x60);
  lVar9 = lVar1 + *(long *)(param_1 + 0x28);
  *param_6 = 0xffffffff;
  param_6[3] = 0;
  *(undefined8 *)(param_6 + 0xc) = 0;
  *(undefined8 *)(param_6 + 0xe) = 0;
  *(undefined8 *)(param_6 + 0x10) = 0;
  *(undefined8 *)(param_6 + 0x12) = 0;
  *(undefined8 *)(param_6 + 0x14) = 0;
  param_6[2] = param_4;
  *(long *)(param_6 + 0x18) = lVar1 + lVar5;
  *(long *)(param_6 + 10) = lVar9;
  iVar8 = param_4;
  if (lVar11 != 0) {
    lVar11 = lVar11 + lVar1;
    *(long *)(param_6 + 0x16) = lVar11;
    if (param_4 < 1) {
      iVar8 = 0;
    }
    else {
      piVar10 = (int *)(lVar11 + 4);
      iVar8 = 0;
      piVar2 = (int *)(lVar11 + 8 + (ulong)(param_4 - 1) * 4);
      do {
        while (lVar11 != 0) {
          iVar4 = *piVar10;
          piVar10 = piVar10 + 1;
          iVar8 = iVar8 + iVar4;
          if (piVar10 == piVar2) goto LAB_009fd8a0;
        }
        piVar10 = piVar10 + 1;
        iVar8 = iVar8 + 1;
      } while (piVar10 != piVar2);
    }
  }
LAB_009fd8a0:
  lVar11 = (long)param_3 * 8 + -8;
  *(long *)(param_6 + 0xe) = lVar11 + *(long *)(param_1 + 0x38);
  lVar5 = (long)param_5 * 4 + -4;
  *(long *)(param_6 + 0x10) = lVar5 + *(long *)(param_1 + 0x40);
  if (*(long *)(param_1 + 0x48) != 0) {
    lVar6 = *(long *)(param_1 + 0x50);
    *(long *)(param_6 + 0x12) = lVar11 + *(long *)(param_1 + 0x48);
    *(long *)(param_6 + 0x14) = lVar5 + lVar6;
  }
  uVar7 = sumIVectorElems_vsdlss(lVar9,param_4);
  if ((uVar7 & 1) != 0) {
    nrerror_vsdlss(1,"mldorder.c",0x48c);
  }
  param_6[3] = (int)uVar7 / 2;
  *param_7 = iVar8;
  return param_2 + lVar1;
}


