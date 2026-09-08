/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteHashTable_vsdlss  addr: 00a2f820  size: 57 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteHashTable_vsdlss(int *param_1); */

undefined8 deleteHashTable_vsdlss(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 != (int *)0x0) {
    iVar1 = *param_1;
    iVar2 = 1;
    if (0 < iVar1) {
      do {
        iVar3 = iVar2 + 1;
        deleteBucketFromHt_vsdlss(iVar2,param_1);
        iVar2 = iVar3;
      } while (iVar3 <= iVar1);
    }
    if (*(long *)(param_1 + 2) != 0) {
      casiFree_vsdlss();
    }
    param_1[2] = 0;
    param_1[3] = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


