/* VSDLSS function (decompiled by Ghidra) */
/* name: computeValBuffSize_vsdlss  addr: 009f2300  size: 18 */
#include "vsdlss_ref.h"

/* signature: int computeValBuffSize_vsdlss(int param_1,int param_2); */

int computeValBuffSize_vsdlss(int param_1,int param_2)

{
  return (param_1 + 1) * param_2 - ((param_2 + -1) * param_2) / 2;
}


