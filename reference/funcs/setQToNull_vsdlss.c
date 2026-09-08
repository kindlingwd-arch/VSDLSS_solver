/* VSDLSS function (decompiled by Ghidra) */
/* name: setQToNull_vsdlss  addr: 00a0d660  size: 17 */
#include "vsdlss_ref.h"

/* signature: undefined8 setQToNull_vsdlss(long param_1); */

undefined8 setQToNull_vsdlss(long param_1)

{
  if (param_1 != 0) {
    *(undefined4 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
  }
  return 0;
}


