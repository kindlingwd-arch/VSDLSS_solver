/* VSDLSS function (decompiled by Ghidra) */
/* name: printFloatArray_vsdlss  addr: 009d7890  size: 46 */
#include "vsdlss_ref.h"

/* signature: void printFloatArray_vsdlss(undefined8 *param_1,int param_2); */

void printFloatArray_vsdlss(undefined8 *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 1;
  if (0 < param_2) {
    do {
      param_1 = param_1 + 1;
      iVar1 = iVar1 + 1;
      printf("%f\t",*param_1);
    } while (iVar1 <= param_2);
  }
  putchar(10);
  return;
}


