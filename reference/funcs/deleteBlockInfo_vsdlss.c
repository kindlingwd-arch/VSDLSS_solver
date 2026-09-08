/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteBlockInfo_vsdlss  addr: 00a1ea40  size: 9d */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteBlockInfo_vsdlss(long param_1); */

undefined8 deleteBlockInfo_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 8) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    if (*(long *)(param_1 + 0x10) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    if (*(long *)(param_1 + 0x18) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x18) = 0;
    if (*(long *)(param_1 + 0x20) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x20) = 0;
    if (*(long *)(param_1 + 0x28) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    if (*(long *)(param_1 + 0x30) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x30) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


