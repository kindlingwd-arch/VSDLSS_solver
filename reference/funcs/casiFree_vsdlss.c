/* VSDLSS function (decompiled by Ghidra) */
/* name: casiFree_vsdlss  addr: 009dd480  size: 77 */
#include "vsdlss_ref.h"

/* signature: void casiFree_vsdlss(long param_1); */

void casiFree_vsdlss(long param_1)

{
  bool bVar1;
  long *plVar2;
  long *plVar3;
  long *plVar4;
  
  bVar1 = false;
  plVar3 = (long *)0x0;
  plVar4 = DAT_00e965a0;
  while (DAT_00e965a0 != (long *)0x0) {
    while( true ) {
      plVar2 = DAT_00e965a0;
      if (bVar1) goto LAB_009dd4b1;
      if (*plVar2 == param_1) break;
      bVar1 = false;
      DAT_00e965a0 = (long *)plVar2[1];
      plVar3 = plVar2;
      if ((long *)plVar2[1] == (long *)0x0) goto LAB_009dd4b1;
    }
    DAT_00e965a0 = (long *)plVar2[1];
    if (plVar3 == (long *)0x0) {
      plVar2[1] = (long)DAT_00e965b0;
      bVar1 = true;
      DAT_00e965b0 = plVar2;
      plVar4 = DAT_00e965a0;
    }
    else {
      plVar3[1] = (long)DAT_00e965a0;
      plVar2[1] = (long)DAT_00e965b0;
      bVar1 = true;
      DAT_00e965a0 = (long *)plVar3[1];
      DAT_00e965b0 = plVar2;
    }
  }
LAB_009dd4b1:
  DAT_00e965a0 = plVar4;
  encapsulatedFree_vsdlss();
  return;
}


