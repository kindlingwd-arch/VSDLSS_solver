/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDCoarsen_vsdlss  addr: 009fee40  size: b8 */
#include "vsdlss_ref.h"

/* signature: int MLDCoarsen_vsdlss(int param_1,undefined4 param_2,undefined4 param_3,long param_4,undefined8 param_5); */

int MLDCoarsen_vsdlss(int param_1,undefined4 param_2,undefined4 param_3,long param_4,
                     undefined8 param_5)

{
  long lVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  int iVar5;
  
  lVar4 = 0;
  do {
    iVar5 = (int)lVar4 + 1;
    if (param_1 <= iVar5) {
      nrerror_vsdlss(1,"mldorder.c",0x7f7);
    }
    lVar1 = *(long *)(param_4 + 8 + lVar4 * 8);
    MLDCoarsenOneLevel_vsdlss(param_2,param_3,iVar5,param_4,param_5);
    lVar2 = *(long *)(param_4 + 0x10 + lVar4 * 8);
    iVar3 = (int)lVar4 + 2;
    iVar5 = *(int *)(lVar2 + 8);
    if (iVar5 <= DAT_00ec5d90) {
      return iVar3;
    }
    if ((double)*(int *)(lVar1 + 8) * DAT_00ec5d88 <= (double)iVar5) {
      return iVar3;
    }
    lVar4 = lVar4 + 1;
  } while (iVar5 / 2 < *(int *)(lVar2 + 0xc) * 2);
  return iVar3;
}


