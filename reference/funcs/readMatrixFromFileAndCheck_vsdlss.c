/* VSDLSS function (decompiled by Ghidra) */
/* name: readMatrixFromFileAndCheck_vsdlss  addr: 009f5df0  size: 12 */
#include "vsdlss_ref.h"

/* signature: void readMatrixFromFileAndCheck_vsdlss(undefined8 param_1); */

void readMatrixFromFileAndCheck_vsdlss(undefined8 param_1)

{
  readMatrixFromFile_vsdlss();
  checkInputMatrix_vsdlss(param_1);
  return;
}


