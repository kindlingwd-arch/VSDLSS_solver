/* VSDLSS function (decompiled by Ghidra) */
/* name: newQHeaderOnly_vsdlss  addr: 00a0d600  size: 22 */
#include "vsdlss_ref.h"

/* signature: void newQHeaderOnly_vsdlss(void); */

void newQHeaderOnly_vsdlss(void)

{
  long lVar1;
  
  lVar1 = casiMalloc_vsdlss(0x18);
  *(undefined4 *)(lVar1 + 8) = 0;
  *(undefined8 *)(lVar1 + 0x10) = 0;
  return;
}


