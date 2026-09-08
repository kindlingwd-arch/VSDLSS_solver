/* VSDLSS function (decompiled by Ghidra) */
/* name: closeAllFiles_vsdlss  addr: 009f39e0  size: 33 */
#include "vsdlss_ref.h"

/* signature: void closeAllFiles_vsdlss(void); */

void closeAllFiles_vsdlss(void)

{
  undefined8 *puVar1;
  
  puVar1 = &DAT_00e3c898;
  do {
    if ((FILE *)*puVar1 != (FILE *)0x0) {
      fclose((FILE *)*puVar1);
      *puVar1 = 0;
    }
    puVar1 = puVar1 + 4;
  } while (puVar1 != (undefined8 *)&DAT_00e3d078);
  return;
}


