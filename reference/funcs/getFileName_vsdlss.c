/* VSDLSS function (decompiled by Ghidra) */
/* name: getFileName_vsdlss  addr: 009f3640  size: f */
#include "vsdlss_ref.h"

/* signature: undefined8 getFileName_vsdlss(int param_1); */

undefined8 getFileName_vsdlss(int param_1)

{
  return *(undefined8 *)(&DAT_00e3c860 + (long)param_1 * 0x20);
}


