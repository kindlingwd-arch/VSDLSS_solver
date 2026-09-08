/* VSDLSS function (decompiled by Ghidra) */
/* name: newCountingMarkHeaderOnly_vsdlss  addr: 00a1f240  size: 28 */
#include "vsdlss_ref.h"

/* signature: void newCountingMarkHeaderOnly_vsdlss(void); */

void newCountingMarkHeaderOnly_vsdlss(void)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x10);
  *puVar1 = 0;
  puVar1[1] = 0;
  *(undefined8 *)(puVar1 + 2) = 0;
  return;
}


