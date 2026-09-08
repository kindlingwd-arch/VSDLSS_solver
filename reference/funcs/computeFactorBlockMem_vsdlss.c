/* VSDLSS function (decompiled by Ghidra) */
/* name: computeFactorBlockMem_vsdlss  addr: 00a22550  size: a0 */
#include "vsdlss_ref.h"

/* signature: double computeFactorBlockMem_vsdlss(double param_1,int param_2,long param_3,int param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

double computeFactorBlockMem_vsdlss(double param_1,int param_2,long param_3,int param_4)

{
  int iVar1;
  int *piVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  
  if (param_2 < 1) {
    dVar5 = 0.0;
    dVar4 = _DAT_00b1a140;
  }
  else {
    dVar4 = 0.0;
    piVar2 = (int *)(param_3 + 4);
    dVar5 = 0.0;
    do {
      iVar1 = *piVar2;
      dVar3 = (double)iVar1 * DAT_00ab6798;
      piVar2 = piVar2 + 1;
      dVar6 = (double)iVar1 * DAT_00ad4f20;
      if (dVar4 <= dVar3) {
        dVar4 = dVar3;
      }
      if (dVar5 <= dVar6) {
        dVar5 = dVar6;
      }
    } while (piVar2 != (int *)(param_3 + 8 + (ulong)(param_2 - 1) * 4));
    dVar4 = dVar4 + DAT_00ab6798 + DAT_00ab6798;
  }
  dVar4 = ((double)param_4 * dVar4 + dVar5) * DAT_00ab7248;
  if ((double)param_2 * _DAT_00b24350 <= dVar4) {
    dVar4 = (double)param_2 * _DAT_00b24350;
  }
  if (dVar4 <= param_1 * DAT_00abc268) {
    dVar4 = param_1 * DAT_00abc268;
  }
  return dVar4;
}


