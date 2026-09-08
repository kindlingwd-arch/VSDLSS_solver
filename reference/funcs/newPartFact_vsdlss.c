/* VSDLSS function (decompiled by Ghidra) */
/* name: newPartFact_vsdlss  addr: 00a08410  size: 7e */
#include "vsdlss_ref.h"

/* signature: undefined4 * newPartFact_vsdlss(int param_1); */

undefined4 * newPartFact_vsdlss(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  
  lVar5 = (long)param_1;
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x20);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar3 = (undefined4 *)tagNewIVector_vsdlss(uVar1,lVar5,1,0);
  *(undefined4 **)(puVar2 + 2) = puVar3;
  *puVar3 = 0;
  puVar4 = (undefined8 *)tagNewIPVector_vsdlss(uVar1,lVar5,1,0);
  *(undefined8 **)(puVar2 + 4) = puVar4;
  *puVar4 = 0;
  puVar4 = (undefined8 *)tagNewDPVector_vsdlss(uVar1,lVar5,1,0);
  *(undefined8 **)(puVar2 + 6) = puVar4;
  *puVar4 = 0;
  return puVar2;
}


