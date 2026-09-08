/* VSDLSS function (decompiled by Ghidra) */
/* name: newBpqHeaderOnly_vsdlss  addr: 00a1ebc0  size: 54 */
#include "vsdlss_ref.h"

/* signature: void newBpqHeaderOnly_vsdlss(void); */

void newBpqHeaderOnly_vsdlss(void)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x30);
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0x7ffffff5;
  puVar1[5] = 0x7ffffff5;
  *(undefined8 *)(puVar1 + 6) = 0;
  *(undefined8 *)(puVar1 + 8) = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  return;
}


