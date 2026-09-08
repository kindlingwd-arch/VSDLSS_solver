/* VSDLSS function (decompiled by Ghidra) */
/* name: initCountingMark_vsdlss  addr: 00a1f1b0  size: 1a */
#include "vsdlss_ref.h"

/* signature: void initCountingMark_vsdlss(int param_1,undefined4 *param_2); */

void initCountingMark_vsdlss(int param_1,undefined4 *param_2)

{
  param_2[1] = param_1;
  *param_2 = 1;
  initIVector_vsdlss(*(undefined8 *)(param_2 + 2),(long)param_1,0);
  return;
}


