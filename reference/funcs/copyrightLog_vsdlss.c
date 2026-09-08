/* VSDLSS function (decompiled by Ghidra) */
/* name: copyrightLog_vsdlss  addr: 009dcd80  size: 45 */
#include "vsdlss_ref.h"

/* signature: void copyrightLog_vsdlss(void); */

void copyrightLog_vsdlss(void)

{
  char *__s;
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)(copyrightLines_vsdlss + 8);
  do {
    while (printToLog_vsdlss(&DAT_00acae72,*puVar1), SILENT_vsdlss != 0) {
      puVar1 = puVar1 + 1;
      if (puVar1 == (undefined8 *)&DAT_00e3be20) {
        return;
      }
    }
    __s = (char *)*puVar1;
    puVar1 = puVar1 + 1;
    puts(__s);
  } while (puVar1 != (undefined8 *)&DAT_00e3be20);
  return;
}


