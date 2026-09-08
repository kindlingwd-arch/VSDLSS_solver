/* VSDLSS function (decompiled by Ghidra) */
/* name: pushAStack_vsdlss  addr: 00a1e810  size: c2 */
#include "vsdlss_ref.h"

/* signature: void pushAStack_vsdlss(int *param_1,undefined4 param_2); */

void pushAStack_vsdlss(int *param_1,undefined4 param_2)

{
  int iVar1;
  long lVar2;
  int iVar3;
  
  iVar3 = param_1[1];
  if (iVar3 == param_1[2]) {
    iVar1 = param_1[3];
    if (iVar3 != iVar1) {
      if (iVar3 * 2 <= iVar1) {
        iVar1 = iVar3 * 2;
      }
      if (*param_1 < 0) {
        lVar2 = newIVector_vsdlss((long)iVar1,0,0);
      }
      else {
        lVar2 = tagNewIVector_vsdlss(*param_1,(long)iVar1,0,0);
      }
      copyIVector_vsdlss(*(undefined8 *)(param_1 + 4),(long)param_1[2],lVar2);
      if ((*param_1 < 0) && (*(long *)(param_1 + 4) != 0)) {
        casiFree_vsdlss();
      }
      *(long *)(param_1 + 4) = lVar2;
      param_1[2] = iVar1;
      iVar3 = param_1[1];
      goto LAB_00a1e82b;
    }
    nrerror_vsdlss(0x35,"astackutils.c",100,"AStack");
    iVar3 = param_1[1];
  }
  lVar2 = *(long *)(param_1 + 4);
LAB_00a1e82b:
  param_1[1] = iVar3 + 1;
  *(undefined4 *)(lVar2 + (long)(iVar3 + 1) * 4) = param_2;
  return;
}


