/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteMLDOrderVecs_vsdlss  addr: 009fcac0  size: 1d6 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteMLDOrderVecs_vsdlss(long param_1); */

undefined8 deleteMLDOrderVecs_vsdlss(long param_1)

{
  undefined8 uVar1;
  
  setPqiToNullPqi_vsdlss(*(undefined8 *)(param_1 + 8));
  uVar1 = deletePqi_vsdlss(*(undefined8 *)(param_1 + 8));
  *(undefined8 *)(param_1 + 8) = uVar1;
  setPqiToNullPqi_vsdlss(*(undefined8 *)(param_1 + 0x10));
  uVar1 = deletePqi_vsdlss(*(undefined8 *)(param_1 + 0x10));
  *(undefined8 *)(param_1 + 0x10) = uVar1;
  setHeapToNullHeap(*(undefined8 *)(param_1 + 0x20));
  uVar1 = deleteHeap(*(undefined8 *)(param_1 + 0x20));
  *(undefined8 *)(param_1 + 0x20) = uVar1;
  setHeapToNullHeap(*(undefined8 *)(param_1 + 0x28));
  uVar1 = deleteHeap(*(undefined8 *)(param_1 + 0x28));
  *(undefined8 *)(param_1 + 0x28) = uVar1;
  setQToNull_vsdlss(*(undefined8 *)(param_1 + 0x30));
  uVar1 = deleteQ_vsdlss(*(undefined8 *)(param_1 + 0x30));
  *(undefined8 *)(param_1 + 0x30) = uVar1;
  setBpqToNull_vsdlss(*(undefined8 *)(param_1 + 0x38));
  uVar1 = deleteBpq_vsdlss(*(undefined8 *)(param_1 + 0x38));
  *(undefined8 *)(param_1 + 0x38) = uVar1;
  setCountingMarkToNull_vsdlss(*(undefined8 *)(param_1 + 0x40));
  uVar1 = deleteCountingMark_vsdlss(*(undefined8 *)(param_1 + 0x40));
  *(undefined8 *)(param_1 + 0x40) = uVar1;
  if (*(long *)(param_1 + 0x48) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x48) = 0;
  if (*(long *)(param_1 + 0x50) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x50) = 0;
  if (*(long *)(param_1 + 0x58) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x58) = 0;
  if (*(long *)(param_1 + 0x60) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x60) = 0;
  if (*(long *)(param_1 + 0x68) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x68) = 0;
  if (*(long *)(param_1 + 0x78) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x78) = 0;
  if (*(long *)(param_1 + 0x80) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x80) = 0;
  if (*(long *)(param_1 + 0x90) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x90) = 0;
  if (*(long *)(param_1 + 0x98) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0x98) = 0;
  if (*(long *)(param_1 + 0xa0) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0xa0) = 0;
  if (*(long *)(param_1 + 0xa8) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0xa8) = 0;
  if (*(long *)(param_1 + 0xb0) != 0) {
    casiFree_vsdlss();
  }
  *(undefined8 *)(param_1 + 0xb0) = 0;
  casiFree_vsdlss(param_1);
  return 0;
}


