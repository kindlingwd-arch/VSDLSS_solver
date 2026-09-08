/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteAllTempTags_vsdlss  addr: 009fba70  size: 3e */
#include "vsdlss_ref.h"

/* signature: void deleteAllTempTags_vsdlss(void); */

void deleteAllTempTags_vsdlss(void)

{
  ulong uVar1;
  ulong uVar2;
  
  uVar1 = 0;
  do {
    while (((&DAT_00ec4180)[uVar1] != 0 && ((&DAT_00ec5980)[uVar1] == 0))) {
      uVar2 = uVar1 & 0xffffffff;
      uVar1 = uVar1 + 1;
      deleteTag_vsdlss(uVar2);
      if (uVar1 == 0x100) {
        return;
      }
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 != 0x100);
  return;
}


