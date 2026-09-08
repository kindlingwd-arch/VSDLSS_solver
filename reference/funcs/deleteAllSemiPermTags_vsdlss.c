/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteAllSemiPermTags_vsdlss  addr: 009fbba0  size: 3f */
#include "vsdlss_ref.h"

/* signature: void deleteAllSemiPermTags_vsdlss(void); */

void deleteAllSemiPermTags_vsdlss(void)

{
  ulong uVar1;
  ulong uVar2;
  
  uVar1 = 0;
  do {
    while (((&DAT_00ec4180)[uVar1] != 0 && ((&DAT_00ec5980)[uVar1] == 1))) {
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


