/* VSDLSS function (decompiled by Ghidra) */
/* name: getFormat_vsdlss  addr: 009f3670  size: e */
#include "vsdlss_ref.h"

/* signature: undefined4 getFormat_vsdlss(int param_1); */

undefined4 getFormat_vsdlss(int param_1)

{
  return *(undefined4 *)(&DAT_00e3c868 + (long)param_1 * 0x20);
}


