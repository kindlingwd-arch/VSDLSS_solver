/* VSDLSS function (decompiled by Ghidra) */
/* name: casiTimes_vsdlss  addr: 009e1e50  size: 26 */
#include "vsdlss_ref.h"

/* signature: void casiTimes_vsdlss(clock_t *param_1); */

void casiTimes_vsdlss(clock_t *param_1)

{
  tms local_28;
  
  times(&local_28);
  *param_1 = local_28.tms_utime;
  param_1[1] = local_28.tms_stime;
  return;
}


