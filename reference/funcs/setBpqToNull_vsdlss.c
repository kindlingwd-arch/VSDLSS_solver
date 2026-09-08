/* VSDLSS function (decompiled by Ghidra) */
/* name: setBpqToNull_vsdlss  addr: 00a1ec80  size: 49 */
#include "vsdlss_ref.h"

/* signature: undefined8 setBpqToNull_vsdlss(undefined4 *param_1); */

undefined8 setBpqToNull_vsdlss(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0x7ffffff5;
    param_1[5] = 0x7ffffff5;
    *(undefined8 *)(param_1 + 6) = 0;
    *(undefined8 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 10) = 0;
  }
  return 0;
}


