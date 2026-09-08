/* VSDLSS function (decompiled by Ghidra) */
/* name: newMLDGraphHeaderOnly_vsdlss  addr: 009fc520  size: 6e */
#include "vsdlss_ref.h"

/* signature: void newMLDGraphHeaderOnly_vsdlss(void); */

void newMLDGraphHeaderOnly_vsdlss(void)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x68);
  *puVar1 = 0xffffffff;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  *(undefined8 *)(puVar1 + 0xe) = 0;
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x12) = 0;
  *(undefined8 *)(puVar1 + 0x14) = 0;
  *(undefined8 *)(puVar1 + 0x16) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  return;
}


