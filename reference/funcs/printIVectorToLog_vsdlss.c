/* VSDLSS function (decompiled by Ghidra) */
/* name: printIVectorToLog_vsdlss  addr: 009f8c90  size: 86 */
#include "vsdlss_ref.h"

/* signature: void printIVectorToLog_vsdlss(undefined8 param_1,undefined4 *param_2,int param_3); */

void printIVectorToLog_vsdlss(undefined8 param_1,undefined4 *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 1;
  printToLog_vsdlss("\n  Begin %s of Size: %d\n",param_1);
  printToLog_vsdlss("\tIndex\tValue\n");
  printToLog_vsdlss("\t-----\t-----\n");
  if (0 < param_3) {
    do {
      param_2 = param_2 + 1;
      iVar2 = iVar1 + 1;
      printToLog_vsdlss(&DAT_00b23e64,iVar1,*param_2);
      iVar1 = iVar2;
    } while (iVar2 <= param_3);
  }
  printToLog_vsdlss("  End   %s\n\n",param_1);
  return;
}


