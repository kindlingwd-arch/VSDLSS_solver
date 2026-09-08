/* VSDLSS function (decompiled by Ghidra) */
/* name: myIntegerCompare_vsdlss  addr: 009d78e0  size: 12 */
#include "vsdlss_ref.h"

/* signature: ulong myIntegerCompare_vsdlss(int *param_1,int *param_2); */

ulong myIntegerCompare_vsdlss(int *param_1,int *param_2)

{
  ulong uVar1;
  
  uVar1 = 0xffffffff;
  if (*param_2 <= *param_1) {
    uVar1 = (ulong)(*param_1 != *param_2);
  }
  return uVar1;
}


