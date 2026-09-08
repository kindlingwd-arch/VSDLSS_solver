/* VSDLSS function (decompiled by Ghidra) */
/* name: cwCasiCasmsg_vsdlss  addr: 009dcdf0  size: 22 */
#include "vsdlss_ref.h"

/* signature: void cwCasiCasmsg_vsdlss(undefined8 param_1); */

void cwCasiCasmsg_vsdlss(undefined8 param_1)

{
  if (SILENT_vsdlss != 0) {
    return;
  }
  fprintf(stdout,"%s\n",param_1);
  return;
}


