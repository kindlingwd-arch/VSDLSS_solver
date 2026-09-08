/* VSDLSS function (decompiled by Ghidra) */
/* name: newCEdgeChunk_vsdlss  addr: 009e1f10  size: 57 */
#include "vsdlss_ref.h"

/* signature: void newCEdgeChunk_vsdlss(void); */

void newCEdgeChunk_vsdlss(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = newBlock_vsdlss(DAT_00e3be28,DAT_00ead890 << 4);
  if (DAT_00ead890 != 1) {
    lVar1 = lVar2 + -0x10 + DAT_00ead890 * 0x10;
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


