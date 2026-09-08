/* VSDLSS function (decompiled by Ghidra) */
/* name: addTimes_vsdlss  addr: 009d7970  size: 16 */
#include "vsdlss_ref.h"

/* signature: void addTimes_vsdlss(long *param_1,long *param_2,long *param_3); */

void addTimes_vsdlss(long *param_1,long *param_2,long *param_3)

{
  *param_3 = *param_2 + *param_1;
  param_3[1] = param_2[1] + param_1[1];
  return;
}


