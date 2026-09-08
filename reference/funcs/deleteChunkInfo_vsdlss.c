/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteChunkInfo_vsdlss  addr: 009e2450  size: 159 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteChunkInfo_vsdlss(long param_1); */

undefined8 deleteChunkInfo_vsdlss(long param_1)

{
  int iVar1;
  long *plVar2;
  long lVar3;
  int iVar4;
  
  if (param_1 != 0) {
    lVar3 = 8;
    iVar4 = 1;
    if (0 < *(int *)(param_1 + 0xc)) {
      do {
        plVar2 = (long *)(lVar3 + *(long *)(param_1 + 0x18));
        if (*plVar2 != 0) {
          casiFree_vsdlss();
          plVar2 = (long *)(lVar3 + *(long *)(param_1 + 0x18));
        }
        iVar4 = iVar4 + 1;
        lVar3 = lVar3 + 8;
        iVar1 = *(int *)(param_1 + 0xc);
        *plVar2 = 0;
      } while (iVar4 <= iVar1);
    }
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
    if (*(long *)(param_1 + 0x38) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    if (*(long *)(param_1 + 0x40) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x40) = 0;
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
    casiFree_vsdlss(param_1);
  }
  return 0;
}


