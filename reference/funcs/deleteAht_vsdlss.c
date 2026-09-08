/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteAht_vsdlss  addr: 00a2f560  size: 6d */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteAht_vsdlss(long param_1); */

undefined8 deleteAht_vsdlss(long param_1)

{
  if (param_1 != 0) {
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
    casiFree_vsdlss(param_1);
  }
  return 0;
}


