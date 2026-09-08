/* VSDLSS function (decompiled by Ghidra) */
/* name: printOffDiagonalsToLog_vsdlss  addr: 009f8e40  size: 98 */
#include "vsdlss_ref.h"

/* signature: void printOffDiagonalsToLog_vsdlss(long param_1,long param_2,long param_3,int param_4); */

void printOffDiagonalsToLog_vsdlss(long param_1,long param_2,long param_3,int param_4)

{
  long lVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  
  lVar4 = 0;
  printToLog_vsdlss("\n  Begin OffDiagonals (Number: %d)\n",param_4);
  printToLog_vsdlss("\tIndex\tFrom\tTo\toffDiagonal\n");
  printToLog_vsdlss("\t-----\t----\t--\t-----------\n");
  iVar5 = 1;
  if (0 < param_4) {
    do {
      puVar2 = (undefined4 *)(param_2 + 4 + lVar4);
      puVar3 = (undefined4 *)(param_1 + 4 + lVar4);
      lVar1 = lVar4 * 2;
      iVar6 = iVar5 + 1;
      lVar4 = lVar4 + 4;
      printToLog_vsdlss(*(undefined8 *)(param_3 + 8 + lVar1),&DAT_00b23b89,iVar5,*puVar3,*puVar2);
      iVar5 = iVar6;
    } while (iVar6 <= param_4);
  }
  printToLog_vsdlss("  End   OffDiagonals\n\n");
  return;
}


