/* VSDLSS function (decompiled by Ghidra) */
/* name: newTime_vsdlss  addr: 009d7910  size: 1d */
#include "vsdlss_ref.h"

/* signature: void newTime_vsdlss(void); */

void newTime_vsdlss(void)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)newTimerFromPool();
  *puVar1 = 0;
  puVar1[1] = 0;
  return;
}


