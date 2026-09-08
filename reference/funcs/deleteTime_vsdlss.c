/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteTime_vsdlss  addr: 009d7930  size: 18 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteTime_vsdlss(long param_1); */

undefined8 deleteTime_vsdlss(long param_1)

{
  if (param_1 != 0) {
    freeTimerFromPool();
    return 0;
  }
  return 0;
}


