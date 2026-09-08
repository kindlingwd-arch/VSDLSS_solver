/* VSDLSS function (decompiled by Ghidra) */
/* name: printCopyright_vsdlss  addr: 009f5120  size: 36 */
#include "vsdlss_ref.h"

/* signature: void printCopyright_vsdlss(int param_1); */

void printCopyright_vsdlss(int param_1)

{
  long lVar1;
  int iVar2;
  
  iVar2 = 1;
  if (0 < param_1) {
    do {
      lVar1 = (long)iVar2;
      iVar2 = iVar2 + 1;
      cwCasiCasmsg_vsdlss(*(undefined8 *)(copyrightLines_vsdlss + lVar1 * 8));
    } while (iVar2 <= param_1);
  }
  return;
}


