/* VSDLSS function (decompiled by Ghidra) */
/* name: getNBlocksPartFact_vsdlss  addr: 00a1ab50  size: 64 */
#include "vsdlss_ref.h"

/* signature: int getNBlocksPartFact_vsdlss(int param_1,long param_2,int param_3); */

int getNBlocksPartFact_vsdlss(int param_1,long param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  if (0 < param_3) {
    if (0 < param_1) {
      iVar3 = 1;
      iVar2 = *(int *)(param_2 + (long)param_1 * 4);
      if (param_3 == iVar2) {
        return 1;
      }
      if (iVar2 < param_3) {
        piVar1 = (int *)(param_2 + -4 + (long)param_1 * 4);
        do {
          if (iVar3 == param_1) break;
          iVar2 = iVar2 + *piVar1;
          iVar3 = iVar3 + 1;
          if (param_3 == iVar2) {
            return iVar3;
          }
          piVar1 = piVar1 + -1;
        } while (iVar2 < param_3);
      }
    }
    nrerror_vsdlss(1,"vsdlss1memutils.c",0xbc4);
  }
  return iVar3;
}


