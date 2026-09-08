/* VSDLSS function (decompiled by Ghidra) */
/* name: readMatrixFromFilePrintToLog_vsdlss  addr: 009f5a00  size: 137 */
#include "vsdlss_ref.h"

/* signature: void readMatrixFromFilePrintToLog_vsdlss(int *param_1); */

void readMatrixFromFilePrintToLog_vsdlss(int *param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  iVar1 = param_1[1];
  iVar2 = *param_1;
  printToLog_vsdlss("  Begin Contents of Matrix Files\n");
  printToLog_vsdlss("  ------------------------------\n");
  printToLog_vsdlss("\tNumber of Nodes(Variables): %d\n",iVar1);
  printToLog_vsdlss("\tNumber of Offdiagonals: %d\n",iVar2);
  lVar3 = newFVector_vsdlss(0,(long)iVar1,0);
  readFVectorFNum_vsdlss(0x1a,iVar1,lVar3);
  printFVectorToLog_vsdlss("Diagonal Vector",lVar3,iVar1);
  if (lVar3 != 0) {
    casiFree_vsdlss(lVar3);
  }
  lVar5 = (long)iVar2;
  lVar3 = newIVector_vsdlss(lVar5,0,0);
  readIVectorFNum_vsdlss(0x1b,iVar2,lVar3);
  lVar4 = newIVector_vsdlss(lVar5,0,0);
  readIVectorFNum_vsdlss(0x1c,iVar2,lVar4);
  lVar5 = newFVector_vsdlss(0,lVar5,0);
  readFVectorFNum_vsdlss(0x1d,iVar2,lVar5);
  printOffDiagonalsToLog_vsdlss(lVar3,lVar4,lVar5,iVar2);
  if (lVar3 != 0) {
    casiFree_vsdlss(lVar3);
  }
  if (lVar4 != 0) {
    casiFree_vsdlss(lVar4);
  }
  if (lVar5 != 0) {
    casiFree_vsdlss(lVar5);
  }
  printToLog_vsdlss("  End   Contents of Matrix Files\n");
  printToLog_vsdlss("  ------------------------------\n");
  return;
}


