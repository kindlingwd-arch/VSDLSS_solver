/* VSDLSS function (decompiled by Ghidra) */
/* name: setFileName_vsdlss  addr: 009f35f0  size: 4b */
#include "vsdlss_ref.h"

/* signature: void setFileName_vsdlss(int param_1,char *param_2); */

void setFileName_vsdlss(int param_1,char *param_2)

{
  if (*(int *)(&DAT_00e3c86c + (long)param_1 * 0x20) != 3) {
    nrerror_vsdlss(0x18,"fmanager.c",0x73);
  }
  strcpy(*(char **)(&DAT_00e3c860 + (long)param_1 * 0x20),param_2);
  return;
}


