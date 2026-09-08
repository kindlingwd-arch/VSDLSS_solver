/* VSDLSS function (decompiled by Ghidra) */
/* name: removeAllChunkFiles_vsdlss  addr: 009e2bf0  size: 41 */
#include "vsdlss_ref.h"

/* signature: void removeAllChunkFiles_vsdlss(char *param_1); */

void removeAllChunkFiles_vsdlss(char *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (((param_1 != (char *)0x0) && (iVar1 = *(int *)(param_1 + 8), *param_1 != '\0')) && (0 < iVar1)
     ) {
    iVar2 = 1;
    do {
      removeFactorChunkFile_vsdlss(param_1,iVar2);
      iVar3 = iVar2 + 1;
      removeAChunkFile_vsdlss(param_1,iVar2);
      iVar2 = iVar3;
    } while (iVar3 <= iVar1);
  }
  return;
}


