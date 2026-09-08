/* VSDLSS function (decompiled by Ghidra) */
/* name: deletePq_vsdlss  addr: 00a2c600  size: 1a */
#include "vsdlss_ref.h"

/* signature: undefined8 deletePq_vsdlss(undefined4 *param_1); */

undefined8 deletePq_vsdlss(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    deleteTag_vsdlss(*param_1);
    return 0;
  }
  return 0;
}


