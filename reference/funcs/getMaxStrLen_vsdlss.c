/* VSDLSS function (decompiled by Ghidra) */
/* name: getMaxStrLen_vsdlss  addr: 009d9390  size: 72 */
#include "vsdlss_ref.h"

/* signature: ulong getMaxStrLen_vsdlss(long param_1,int param_2); */

ulong getMaxStrLen_vsdlss(long param_1,int param_2)

{
  size_t sVar1;
  undefined8 *puVar2;
  ulong uVar3;
  
  if ((0 < param_2) && (param_1 != 0)) {
    sVar1 = strlen(*(char **)(param_1 + 8));
    uVar3 = sVar1 & 0xffffffff;
    if (param_2 != 1) {
      puVar2 = (undefined8 *)(param_1 + 0x10);
      do {
        sVar1 = strlen((char *)*puVar2);
        if ((int)uVar3 < (int)sVar1) {
          uVar3 = sVar1 & 0xffffffff;
        }
        puVar2 = puVar2 + 1;
      } while (puVar2 != (undefined8 *)(param_1 + 0x18 + (ulong)(param_2 - 2) * 8));
    }
    return uVar3;
  }
  return 0;
}


