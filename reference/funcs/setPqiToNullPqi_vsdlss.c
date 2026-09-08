/* VSDLSS function (decompiled by Ghidra) */
/* name: setPqiToNullPqi_vsdlss  addr: 00a0cb50  size: 2e */
#include "vsdlss_ref.h"

/* signature: undefined8 setPqiToNullPqi_vsdlss(long param_1); */

undefined8 setPqiToNullPqi_vsdlss(long param_1)

{
  if (param_1 != 0) {
    *(undefined4 *)(param_1 + 4) = 0;
    *(undefined4 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 *)(param_1 + 0x20) = 0;
  }
  return 0;
}


