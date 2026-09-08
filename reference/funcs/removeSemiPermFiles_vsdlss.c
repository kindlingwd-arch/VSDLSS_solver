/* VSDLSS function (decompiled by Ghidra) */
/* name: removeSemiPermFiles_vsdlss  addr: 009f3a70  size: 41 */
#include "vsdlss_ref.h"

/* signature: void removeSemiPermFiles_vsdlss(void); */

void removeSemiPermFiles_vsdlss(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = &DAT_00e3c88c;
  iVar1 = 1;
  do {
    while (*piVar3 == 1) {
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


