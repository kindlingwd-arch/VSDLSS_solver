/* VSDLSS function (decompiled by Ghidra) */
/* name: setReduceProblemSizeF_vsdlss  addr: 00a12800  size: 19 */
#include "vsdlss_ref.h"

/* signature: void setReduceProblemSizeF_vsdlss(int param_1); */

void setReduceProblemSizeF_vsdlss(int param_1)

{
  int iVar1;
  
  iVar1 = 2;
  if (param_1 < 3) {
    iVar1 = param_1;
  }
  DAT_00e3d074 = 0;
  if (-1 < iVar1) {
    DAT_00e3d074 = iVar1;
  }
  return;
}


