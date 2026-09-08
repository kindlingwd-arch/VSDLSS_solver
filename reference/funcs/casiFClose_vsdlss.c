/* VSDLSS function (decompiled by Ghidra) */
/* name: casiFClose_vsdlss  addr: 009f3950  size: 2a */
#include "vsdlss_ref.h"

/* signature: void casiFClose_vsdlss(int param_1); */

void casiFClose_vsdlss(int param_1)

{
  if (*(FILE **)(&DAT_00e3c878 + (long)param_1 * 0x20) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3c878 + (long)param_1 * 0x20));
    *(undefined8 *)(&DAT_00e3c878 + (long)param_1 * 0x20) = 0;
  }
  return;
}


