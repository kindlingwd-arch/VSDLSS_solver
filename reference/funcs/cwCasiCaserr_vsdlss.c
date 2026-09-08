/* VSDLSS function (decompiled by Ghidra) */
/* name: cwCasiCaserr_vsdlss  addr: 009dcdd0  size: 18 */
#include "vsdlss_ref.h"

/* signature: void cwCasiCaserr_vsdlss(ulong param_1,undefined8 param_2); */

void cwCasiCaserr_vsdlss(ulong param_1,undefined8 param_2)

{
  fprintf(stderr,"Level:%d\n%s\n",param_1 & 0xffffffff,param_2);
  return;
}


