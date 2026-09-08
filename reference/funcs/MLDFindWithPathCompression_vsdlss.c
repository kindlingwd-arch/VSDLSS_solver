/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindWithPathCompression_vsdlss  addr: 00a28040  size: a2 */
#include "vsdlss_ref.h"

/* signature: int MLDFindWithPathCompression_vsdlss(int param_1,long param_2); */

int MLDFindWithPathCompression_vsdlss(int param_1,long param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  
  piVar1 = (int *)(param_2 + (long)param_1 * 4);
  iVar4 = *piVar1;
  piVar6 = piVar1;
  iVar5 = param_1;
  if (iVar4 != 0) {
    do {
      if (iVar5 == iVar4) {
        nrerror_vsdlss(1,"mldmmd.c",0x5e);
      }
      iVar5 = *piVar6;
      piVar6 = (int *)(param_2 + (long)iVar5 * 4);
      iVar4 = *piVar6;
    } while (iVar4 != 0);
    iVar3 = *piVar1;
    iVar4 = param_1;
    while (iVar2 = iVar3, iVar2 != 0) {
      *piVar1 = iVar5;
      piVar1 = (int *)(param_2 + (long)iVar2 * 4);
      iVar4 = iVar2;
      iVar3 = *piVar1;
    }
    param_1 = iVar5;
    if (iVar5 != iVar4) {
      nrerror_vsdlss(1,"mldmmd.c",0x6e);
    }
  }
  return param_1;
}


