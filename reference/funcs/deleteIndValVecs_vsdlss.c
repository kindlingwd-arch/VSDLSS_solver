/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteIndValVecs_vsdlss  addr: 00a082e0  size: 3f */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteIndValVecs_vsdlss(long *param_1); */

undefined8 deleteIndValVecs_vsdlss(long *param_1)

{
  if (param_1 != (long *)0x0) {
    if (*param_1 != 0) {
      casiFree_vsdlss();
    }
    *param_1 = 0;
    if (param_1[1] != 0) {
      casiFree_vsdlss();
    }
    param_1[1] = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


