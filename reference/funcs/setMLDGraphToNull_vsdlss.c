/* VSDLSS function (decompiled by Ghidra) */
/* name: setMLDGraphToNull_vsdlss  addr: 00a02db0  size: 54 */
#include "vsdlss_ref.h"

/* signature: void setMLDGraphToNull_vsdlss(long param_1); */

void setMLDGraphToNull_vsdlss(long param_1)

{
  if (param_1 != 0) {
    *(undefined8 *)(param_1 + 0x28) = 0;
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined8 *)(param_1 + 0x40) = 0;
    *(undefined8 *)(param_1 + 0x48) = 0;
    *(undefined8 *)(param_1 + 0x50) = 0;
    *(undefined8 *)(param_1 + 0x58) = 0;
    *(undefined8 *)(param_1 + 0x60) = 0;
    *(undefined4 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0xc) = 0;
    *(undefined4 *)(param_1 + 4) = 0;
  }
  return;
}


