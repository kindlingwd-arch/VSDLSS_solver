/* VSDLSS function (decompiled by Ghidra) */
/* name: printDVectorToLog_vsdlss  addr: 009f8d20  size: 8b */
#include "vsdlss_ref.h"

/* signature: void printDVectorToLog_vsdlss(undefined8 param_1,undefined8 *param_2,int param_3); */

void printDVectorToLog_vsdlss(undefined8 param_1,undefined8 *param_2,int param_3)

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
      printToLog_vsdlss(*param_2,"\t%d\t%11.8g\n",iVar1);
      iVar1 = iVar2;
    } while (iVar2 <= param_3);
  }
  printToLog_vsdlss("  End   %s\n\n",param_1);
  return;
}


