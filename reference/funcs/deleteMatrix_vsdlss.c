/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteMatrix_vsdlss  addr: 009e5d80  size: 5d */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteMatrix_vsdlss(long *param_1,int param_2); */

undefined8 deleteMatrix_vsdlss(long *param_1,int param_2)

{
  long *plVar1;
  int iVar2;
  
  if (param_1 != (long *)0x0) {
    iVar2 = 1;
    plVar1 = param_1;
    if (0 < param_2) {
      do {
        plVar1 = plVar1 + 1;
        if (*plVar1 != 0) {
          casiFree_vsdlss();
        }
        iVar2 = iVar2 + 1;
        *plVar1 = 0;
      } while (iVar2 <= param_2);
    }
    casiFree_vsdlss(param_1);
  }
  return 0;
}


