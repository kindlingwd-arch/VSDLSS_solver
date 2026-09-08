/* VSDLSS function (decompiled by Ghidra) */
/* name: newCharPtrChunk_vsdlss  addr: 009dd240  size: 57 */
#include "vsdlss_ref.h"

/* signature: void newCharPtrChunk_vsdlss(void); */

void newCharPtrChunk_vsdlss(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = newBlock_vsdlss(DAT_00e3be20,DAT_00e965a8 << 4);
  if (DAT_00e965a8 != 1) {
    lVar1 = lVar2 + -0x10 + DAT_00e965a8 * 0x10;
    lVar4 = lVar2;
    do {
      lVar3 = lVar4 + 0x10;
      *(long *)(lVar4 + 8) = lVar3;
      lVar2 = lVar1;
      lVar4 = lVar3;
    } while (lVar3 != lVar1);
  }
  *(undefined8 *)(lVar2 + 8) = 0;
  return;
}


