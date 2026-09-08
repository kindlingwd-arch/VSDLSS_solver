/* VSDLSS function (decompiled by Ghidra) */
/* name: getMyThreadIndex_vsdlss  addr: 009e1120  size: 42 */
#include "vsdlss_ref.h"

/* signature: int getMyThreadIndex_vsdlss(long param_1); */

int getMyThreadIndex_vsdlss(long param_1)

{
  int iVar1;
  long lVar2;
  int iVar3;
  
  if (0 < DAT_00ead1a4) {
    lVar2 = 0;
    iVar3 = 1;
    do {
      if ((*(int *)((long)&DAT_00ead0a4 + lVar2) != 0) &&
         (*(long *)((long)&DAT_00eac1c8 + lVar2 * 2) == param_1)) {
        iVar1 = 0;
        if (iVar3 <= DAT_00ead1a4) {
          iVar1 = iVar3;
        }
        return iVar1;
      }
      iVar3 = iVar3 + 1;
      lVar2 = lVar2 + 4;
    } while (iVar3 <= DAT_00ead1a4);
  }
  return 0;
}


