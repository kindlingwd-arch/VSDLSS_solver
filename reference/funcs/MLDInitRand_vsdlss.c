/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDInitRand_vsdlss  addr: 009fbd90  size: 12 */
#include "vsdlss_ref.h"

/* signature: void MLDInitRand_vsdlss(uint param_1); */

void MLDInitRand_vsdlss(uint param_1)

{
  if (param_1 != 0) {
    srand(param_1);
    return;
  }
  srand(0x10e1);
  return;
}


