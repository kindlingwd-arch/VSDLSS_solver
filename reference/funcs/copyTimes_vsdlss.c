/* VSDLSS function (decompiled by Ghidra) */
/* name: copyTimes_vsdlss  addr: 009d7a00  size: f */
#include "vsdlss_ref.h"

/* signature: void copyTimes_vsdlss(undefined8 *param_1,undefined8 *param_2); */

void copyTimes_vsdlss(undefined8 *param_1,undefined8 *param_2)

{
  *param_2 = *param_1;
  param_2[1] = param_1[1];
  return;
}


