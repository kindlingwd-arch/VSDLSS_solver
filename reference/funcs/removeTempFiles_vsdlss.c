/* VSDLSS function (decompiled by Ghidra) */
/* name: removeTempFiles_vsdlss  addr: 009f3a20  size: 41 */
#include "vsdlss_ref.h"

/* signature: void removeTempFiles_vsdlss(void); */

void removeTempFiles_vsdlss(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = &DAT_00e3c88c;
  iVar1 = 1;
  do {
    while (*piVar3 == 2) {
      iVar2 = iVar1 + 1;
      piVar3 = piVar3 + 8;
      casiRemove_vsdlss(iVar1);
      iVar1 = iVar2;
      if (iVar2 == 0x40) {
        return;
      }
    }
    iVar1 = iVar1 + 1;
    piVar3 = piVar3 + 8;
  } while (iVar1 != 0x40);
  return;
}


