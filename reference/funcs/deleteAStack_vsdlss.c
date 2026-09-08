/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteAStack_vsdlss  addr: 00a1e7c0  size: 47 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteAStack_vsdlss(int *param_1); */

undefined8 deleteAStack_vsdlss(int *param_1)

{
  if (param_1 != (int *)0x0) {
    if (-1 < *param_1) {
      nrerror_vsdlss(1,"astackutils.c",0x4b);
    }
    if (*(long *)(param_1 + 4) != 0) {
      casiFree_vsdlss();
    }
    param_1[4] = 0;
    param_1[5] = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


