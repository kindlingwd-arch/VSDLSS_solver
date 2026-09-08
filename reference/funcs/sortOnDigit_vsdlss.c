/* VSDLSS function (decompiled by Ghidra) */
/* name: sortOnDigit_vsdlss  addr: 009e68a0  size: 15a */
#include "vsdlss_ref.h"

/* signature: void sortOnDigit_vsdlss(long param_1,int param_2,int param_3,long param_4,long param_5,long param_6,long param_7); */

void sortOnDigit_vsdlss(long param_1,int param_2,int param_3,long param_4,long param_5,long param_6,
                       long param_7)

{
  int *piVar1;
  int iVar2;
  ulong uVar3;
  int *piVar4;
  ulong uVar5;
  long lVar6;
  
  uVar5 = (ulong)param_3;
  initIVector_vsdlss(param_5,uVar5,0);
  if (0 < param_2) {
    piVar4 = (int *)(param_1 + 4);
    do {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      piVar1 = (int *)(param_5 + (long)iVar2 * 4);
      *piVar1 = *piVar1 + 1;
    } while (piVar4 != (int *)(param_1 + 8 + (ulong)(param_2 - 1) * 4));
  }
  if ((0 < param_3) && (*(undefined4 *)(param_6 + 4) = 1, 1 < uVar5)) {
    uVar3 = 2;
    do {
      *(int *)(param_6 + uVar3 * 4) =
           *(int *)(param_6 + -4 + uVar3 * 4) + *(int *)(param_5 + -4 + uVar3 * 4);
      uVar3 = uVar3 + 1;
    } while (uVar3 <= uVar5);
  }
  if (0 < param_2) {
    piVar4 = (int *)(param_4 + 4);
    do {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      piVar1 = (int *)(param_6 + (long)*(int *)(param_1 + (long)iVar2 * 4) * 4);
      *(int *)(param_7 + (long)*piVar1 * 4) = iVar2;
      *piVar1 = *piVar1 + 1;
    } while (piVar4 != (int *)(param_4 + 8 + (ulong)(param_2 - 1) * 4));
  }
  if ((*(int *)(param_6 + 4) != *(int *)(param_5 + 4) + 1) ||
     (*(int *)(param_6 + uVar5 * 4) != param_2 + 1)) {
    nrerror_vsdlss(1,"elemarrutils.c",0x110);
  }
  lVar6 = 0;
  if (1 < param_3) {
    do {
      if (*(int *)(param_6 + 8 + lVar6 * 4) !=
          *(int *)(param_5 + 8 + lVar6 * 4) + *(int *)(param_6 + 4 + lVar6 * 4)) {
        nrerror_vsdlss(1,"elemarrutils.c",0x115);
      }
      iVar2 = (int)lVar6;
      lVar6 = lVar6 + 1;
    } while (iVar2 + 3 <= param_3);
  }
  copyIVector_vsdlss(param_7,(long)param_2,param_4);
  return;
}


