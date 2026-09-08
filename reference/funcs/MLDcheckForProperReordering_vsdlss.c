/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDcheckForProperReordering_vsdlss  addr: 00a02e10  size: 118 */
#include "vsdlss_ref.h"

/* signature: void MLDcheckForProperReordering_vsdlss(int param_1,int param_2,long param_3); */

void MLDcheckForProperReordering_vsdlss(int param_1,int param_2,long param_3)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  
  lVar1 = newCVector_vsdlss((long)param_1,0,0);
  if (param_2 != param_1) {
    nrerror_vsdlss(1,"mldorder.c",0xfb6);
  }
  initCVector_vsdlss(lVar1,(long)param_1,0);
  piVar5 = (int *)(param_3 + 4);
  iVar4 = 1;
  if (0 < param_1) {
LAB_00a02e7f:
    do {
      iVar2 = *piVar5;
      if (iVar2 < 0) {
        if (DAT_00ec5df4 != 0) {
          iVar2 = -iVar2;
          goto LAB_00a02e8b;
        }
        nrerror_vsdlss(1,"mldorder.c",0xfc3);
LAB_00a02f00:
        nrerror_vsdlss(1,"mldorder.c",0xfc9);
      }
      else {
LAB_00a02e8b:
        if ((param_1 < iVar2) || (iVar2 < 1)) goto LAB_00a02f00;
      }
      pcVar3 = (char *)(iVar2 + lVar1);
      if (*pcVar3 == '\0') {
        iVar4 = iVar4 + 1;
        piVar5 = piVar5 + 1;
        *pcVar3 = '\x01';
        if (param_1 < iVar4) break;
        goto LAB_00a02e7f;
      }
      iVar4 = iVar4 + 1;
      piVar5 = piVar5 + 1;
      nrerror_vsdlss(1,"mldorder.c",0xfcd);
      *pcVar3 = '\x01';
    } while (iVar4 <= param_1);
  }
  if (lVar1 == 0) {
    return;
  }
  casiFree_vsdlss(lVar1);
  return;
}


