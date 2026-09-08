/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindNewRoot_vsdlss  addr: 009fef00  size: 67 */
#include "vsdlss_ref.h"

/* signature: int MLDFindNewRoot_vsdlss(int param_1,int param_2,long param_3); */

int MLDFindNewRoot_vsdlss(int param_1,int param_2,long param_3)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  
  iVar2 = param_1 + 1;
  if (iVar2 <= param_2) {
    if (*(char *)(param_3 + iVar2) != '\0') {
      pcVar3 = (char *)(param_3 + 1 + (long)iVar2);
      do {
        iVar2 = iVar2 + 1;
        if (param_2 < iVar2) goto LAB_009fef31;
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
      } while (cVar1 != '\0');
    }
    if (iVar2 != -1) {
      return iVar2;
    }
  }
LAB_009fef31:
  iVar2 = -1;
  if (1 < param_1) {
    if (*(char *)(param_3 + 1) == '\0') {
      return 1;
    }
    iVar2 = 1;
    pcVar3 = (char *)(param_3 + 2);
    do {
      iVar2 = iVar2 + 1;
      if (iVar2 == param_1) {
        return -1;
      }
      cVar1 = *pcVar3;
      pcVar3 = pcVar3 + 1;
    } while (cVar1 != '\0');
  }
  return iVar2;
}


