/* VSDLSS function (decompiled by Ghidra) */
/* name: convertToDeciSeconds_vsdlss  addr: 009d7a90  size: 78 */
#include "vsdlss_ref.h"

/* signature: void convertToDeciSeconds_vsdlss(long *param_1); */

void convertToDeciSeconds_vsdlss(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = *param_1;
  lVar2 = sysconf(2);
  lVar1 = param_1[1];
  *param_1 = (long)((double)lVar3 / ((double)lVar2 * DAT_00ab7268));
  lVar3 = sysconf(2);
  param_1[1] = (long)((double)lVar1 / ((double)lVar3 * DAT_00ab7268));
  return;
}


