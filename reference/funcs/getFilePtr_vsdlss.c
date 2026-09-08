/* VSDLSS function (decompiled by Ghidra) */
/* name: getFilePtr_vsdlss  addr: 009f3650  size: f */
#include "vsdlss_ref.h"

/* signature: undefined8 getFilePtr_vsdlss(int param_1); */

undefined8 getFilePtr_vsdlss(int param_1)

{
  return *(undefined8 *)(&DAT_00e3c878 + (long)param_1 * 0x20);
}


