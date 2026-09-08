/* VSDLSS function (decompiled by Ghidra) */
/* name: setCountingMarkToNull_vsdlss  addr: 00a1f300  size: 1d */
#include "vsdlss_ref.h"

/* signature: undefined8 setCountingMarkToNull_vsdlss(undefined4 *param_1); */

undefined8 setCountingMarkToNull_vsdlss(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 1;
    param_1[1] = 0;
    *(undefined8 *)(param_1 + 2) = 0;
  }
  return 0;
}


