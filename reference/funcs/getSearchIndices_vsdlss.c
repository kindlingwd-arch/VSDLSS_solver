/* VSDLSS function (decompiled by Ghidra) */
/* name: getSearchIndices_vsdlss  addr: 009e8d40  size: 1b6 */
#include "vsdlss_ref.h"

/* signature: void getSearchIndices_vsdlss(long param_1,int param_2,int *param_3,int param_4,long param_5); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void getSearchIndices_vsdlss(long param_1,int param_2,int *param_3,int param_4,long param_5)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  int local_3c [3];
  
  if (param_4 < param_2) {
    nrerror_vsdlss(0x25,"factora.c",0x147);
  }
  if ((double)param_4 / (double)param_2 <= _DAT_00b1ab28) {
    if ((param_2 < 1) || (param_4 < 1)) {
      iVar4 = 1;
    }
    else {
      iVar3 = 1;
      iVar4 = 1;
      do {
        param_3 = param_3 + 1;
        lVar5 = (long)iVar4;
        if (*(int *)(param_1 + lVar5 * 4) == *param_3) {
          iVar4 = iVar4 + 1;
          *(int *)(param_5 + lVar5 * 4) = iVar3;
        }
      } while ((iVar4 <= param_2) && (iVar3 = iVar3 + 1, iVar3 <= param_4));
    }
    if (iVar4 <= param_2) {
      nrerror_vsdlss(0x25,"factora.c",0x15b);
      return;
    }
  }
  else if (0 < param_2) {
    lVar5 = 0;
    iVar3 = 0;
    piVar2 = param_3;
    iVar4 = param_4;
    do {
      while( true ) {
        local_3c[0] = *(int *)(param_1 + 4 + lVar5 * 4);
        iVar1 = (int)lVar5;
        if (local_3c[0] == param_3[(long)iVar3 + 1]) break;
        piVar2 = bsearch(local_3c,piVar2 + 1,(long)iVar4,4,myIntegerCompare_vsdlss);
        if (piVar2 == (int *)0x0) {
          nrerror_vsdlss(0x25,"factora.c",0x177);
        }
        iVar3 = (int)((long)piVar2 - (long)param_3 >> 2);
        *(int *)(param_5 + 4 + lVar5 * 4) = iVar3;
        lVar5 = lVar5 + 1;
        iVar4 = param_4 - iVar3;
        if (param_2 < iVar1 + 2) {
          return;
        }
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 1;
      iVar4 = iVar4 + -1;
      *(int *)(param_5 + 4 + lVar5 * 4) = iVar3;
      lVar5 = lVar5 + 1;
    } while (iVar1 + 2 <= param_2);
  }
  return;
}


