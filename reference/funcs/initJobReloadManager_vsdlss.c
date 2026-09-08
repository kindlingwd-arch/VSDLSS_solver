/* VSDLSS function (decompiled by Ghidra) */
/* name: initJobReloadManager_vsdlss  addr: 009f6110  size: 36 */
#include "vsdlss_ref.h"

/* signature: void initJobReloadManager_vsdlss(void); */

void initJobReloadManager_vsdlss(void)

{
  undefined4 *puVar1;
  
  if (DAT_00e3d060 != 0) {
    DAT_00e3d060 = 0;
    puVar1 = &DAT_00eb1f48;
    do {
      *puVar1 = 0;
      puVar1 = puVar1 + 0x122;
    } while (puVar1 != (undefined4 *)&DAT_00ec4148);
  }
  return;
}


