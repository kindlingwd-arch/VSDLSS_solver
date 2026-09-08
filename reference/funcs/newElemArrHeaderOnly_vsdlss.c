/* VSDLSS function (decompiled by Ghidra) */
/* name: newElemArrHeaderOnly_vsdlss  addr: 009e6570  size: 57 */
#include "vsdlss_ref.h"

/* signature: void newElemArrHeaderOnly_vsdlss(undefined4 param_1,undefined4 param_2); */

void newElemArrHeaderOnly_vsdlss(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x40);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  *(undefined1 *)(puVar1 + 2) = 0;
  puVar1[3] = 0;
  *(undefined1 *)(puVar1 + 4) = 0;
  *(undefined8 *)(puVar1 + 6) = 0;
  *(undefined8 *)(puVar1 + 8) = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  *(undefined8 *)(puVar1 + 0xe) = 0;
  return;
}


