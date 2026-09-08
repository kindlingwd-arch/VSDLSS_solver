/* VSDLSS function (decompiled by Ghidra) */
/* name: newPqiHeaderOnly_vsdlss  addr: 00a0cad0  size: 3f */
#include "vsdlss_ref.h"

/* signature: void newPqiHeaderOnly_vsdlss(void); */

void newPqiHeaderOnly_vsdlss(void)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x28);
  *puVar1 = 0xffffffff;
  puVar1[1] = 0;
  puVar1[2] = 0;
  *(undefined8 *)(puVar1 + 4) = 0;
  *(undefined8 *)(puVar1 + 6) = 0;
  *(undefined8 *)(puVar1 + 8) = 0;
  return;
}


