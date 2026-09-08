/* VSDLSS function (decompiled by Ghidra) */
/* name: newMLDOrderVecs_vsdlss  addr: 009fc930  size: 186 */
#include "vsdlss_ref.h"

/* signature: long newMLDOrderVecs_vsdlss(int param_1); */

long newMLDOrderVecs_vsdlss(int param_1)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  
  lVar1 = (long)param_1 * 4 + 4;
  lVar6 = (long)(param_1 * 2 + 4);
  lVar5 = (long)(param_1 + 1);
  lVar2 = casiMalloc_vsdlss(0xb8);
  uVar3 = newPqiHeaderOnly_vsdlss();
  *(undefined8 *)(lVar2 + 8) = uVar3;
  uVar3 = newPqiHeaderOnly_vsdlss();
  *(undefined8 *)(lVar2 + 0x10) = uVar3;
  uVar3 = newHeapHeaderOnly();
  *(undefined8 *)(lVar2 + 0x20) = uVar3;
  uVar3 = newHeapHeaderOnly();
  *(undefined8 *)(lVar2 + 0x28) = uVar3;
  uVar3 = newQHeaderOnly_vsdlss();
  *(undefined8 *)(lVar2 + 0x30) = uVar3;
  uVar3 = newBpqHeaderOnly_vsdlss();
  *(undefined8 *)(lVar2 + 0x38) = uVar3;
  uVar3 = newCountingMarkHeaderOnly_vsdlss();
  *(undefined8 *)(lVar2 + 0x40) = uVar3;
  uVar3 = newCVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x48) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x50) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x58) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x60) = uVar3;
  lVar4 = newIVector_vsdlss(lVar6,1,0);
  *(long *)(lVar2 + 0x68) = lVar4;
  *(long *)(lVar2 + 0x70) = lVar4 + lVar1;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x78) = uVar3;
  lVar4 = newIVector_vsdlss(lVar6,1,0);
  *(long *)(lVar2 + 0x88) = lVar1 + lVar4;
  *(long *)(lVar2 + 0x80) = lVar4;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x90) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0x98) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0xa0) = uVar3;
  uVar3 = newIPVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0xa8) = uVar3;
  uVar3 = newIVector_vsdlss(lVar5,1,0);
  *(undefined8 *)(lVar2 + 0xb0) = uVar3;
  return lVar2;
}


