/* VSDLSS function (decompiled by Ghidra) */
/* name: printElemWithHeading_vsdlss  addr: 009e6760  size: 22 */
#include "vsdlss_ref.h"

/* signature: void printElemWithHeading_vsdlss(undefined8 param_1,undefined4 param_2); */

void printElemWithHeading_vsdlss(undefined8 param_1,undefined4 param_2)

{
  puts("Num\tfromNode\ttoNode\tOffDiag");
  printElem_vsdlss(param_1,param_2);
  return;
}


