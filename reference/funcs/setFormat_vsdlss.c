/* VSDLSS function (decompiled by Ghidra) */
/* name: setFormat_vsdlss  addr: 009f3660  size: e */
#include "vsdlss_ref.h"

/* signature: void setFormat_vsdlss(int param_1,undefined4 param_2); */

void setFormat_vsdlss(int param_1,undefined4 param_2)

{
  *(undefined4 *)(&DAT_00e3c868 + (long)param_1 * 0x20) = param_2;
  return;
}


