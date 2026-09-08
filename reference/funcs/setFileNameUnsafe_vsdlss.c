/* VSDLSS function (decompiled by Ghidra) */
/* name: setFileNameUnsafe_vsdlss  addr: 009f35d0  size: 13 */
#include "vsdlss_ref.h"

/* signature: void setFileNameUnsafe_vsdlss(int param_1,char *param_2); */

void setFileNameUnsafe_vsdlss(int param_1,char *param_2)

{
  strcpy(*(char **)(&DAT_00e3c860 + (long)param_1 * 0x20),param_2);
  return;
}


