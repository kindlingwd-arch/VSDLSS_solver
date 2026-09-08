/* VSDLSS function (decompiled by Ghidra) */
/* name: remFromDeq_vsdlss  addr: 00a2f680  size: 6a */
#include "vsdlss_ref.h"

/* signature: undefined4 remFromDeq_vsdlss(undefined4 *param_1); */

undefined4 remFromDeq_vsdlss(undefined4 *param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  puVar3 = *(undefined4 **)(param_1 + 2);
  puVar4 = *(undefined4 **)(param_1 + 4);
  puVar2 = *(undefined4 **)(puVar3 + 2);
  if (puVar3 != puVar4) {
    if (puVar2 == (undefined4 *)0x0) {
      nrerror_vsdlss(1,"dequtils.c",0x5c);
      puVar3 = *(undefined4 **)(param_1 + 2);
      puVar4 = *(undefined4 **)(param_1 + 4);
    }
    uVar1 = *puVar2;
    *(undefined8 *)(puVar3 + 2) = *(undefined8 *)(puVar2 + 2);
    if (puVar2 == puVar4) {
      *(undefined4 **)(param_1 + 4) = puVar3;
    }
    deleteCEdgelist_vsdlss(puVar2);
    return uVar1;
  }
  return *param_1;
}


