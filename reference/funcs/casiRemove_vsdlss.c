/* VSDLSS function (decompiled by Ghidra) */
/* name: casiRemove_vsdlss  addr: 009f3980  size: 53 */
#include "vsdlss_ref.h"

/* signature: void casiRemove_vsdlss(int param_1); */

void casiRemove_vsdlss(int param_1)

{
  char *__filename;
  long lVar1;
  
  lVar1 = (long)param_1 * 0x20;
  __filename = *(char **)(&DAT_00e3c860 + lVar1);
  if (*(FILE **)(&DAT_00e3c878 + lVar1) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3c878 + lVar1));
    *(undefined8 *)(&DAT_00e3c878 + lVar1) = 0;
  }
  if (*(int *)(&DAT_00e3c86c + (long)param_1 * 0x20) != 3) {
    remove(__filename);
    return;
  }
  return;
}


