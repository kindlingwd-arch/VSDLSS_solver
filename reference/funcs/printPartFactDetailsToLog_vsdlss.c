/* VSDLSS function (decompiled by Ghidra) */
/* name: printPartFactDetailsToLog_vsdlss  addr: 009f90b0  size: 4a8 */
#include "vsdlss_ref.h"

/* signature: void printPartFactDetailsToLog_vsdlss(int param_1,int param_2,int param_3); */

void printPartFactDetailsToLog_vsdlss(int param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 *puVar2;
  FILE *pFVar3;
  size_t sVar4;
  undefined8 *puVar5;
  int iVar6;
  int local_48;
  int local_44;
  int local_40;
  int local_3c [3];
  
  puVar2 = (undefined4 *)newIVector_vsdlss((long)param_1,0,0);
  *puVar2 = 0;
  readIVectorFNum_vsdlss(0x26,param_1,puVar2);
  printIVectorToLog_vsdlss("Permutation Vector",puVar2,param_1);
  casiFree_vsdlss(puVar2);
  pFVar3 = (FILE *)openFileR_vsdlss(0x20);
  sVar4 = fread(&local_48,4,1,pFVar3);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"logfileutils.c",0x167,"printPartFactDetailsToLog:WSize",1,
                   sVar4 & 0xffffffff);
  }
  if (local_48 != param_2) {
    nrerror_vsdlss(1,"logfileutils.c",0x169);
  }
  sVar4 = fread(&local_44,4,1,pFVar3);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"logfileutils.c",0x16e,"printPartFactDetailsToLog:nOffdiagsRed",1,
                   sVar4 & 0xffffffff);
  }
  casiFClose_vsdlss(0x20);
  printToLog_vsdlss("\n  Begin Reduced Matrix: Rows %d ; Number of Offdiags: %d\n",local_48,local_44
                   );
  printToLog_vsdlss("  ----------------------------------------------\n");
  puVar5 = (undefined8 *)newVector_vsdlss(0,(long)local_48,0);
  *puVar5 = 0;
  readDVectorFNum_vsdlss(0x21,local_48,puVar5);
  printDVectorToLog_vsdlss("Diagonal Vec",puVar5,local_48);
  casiFree_vsdlss(puVar5);
  puVar2 = (undefined4 *)newIVector_vsdlss((long)local_44,0,0);
  *puVar2 = 0;
  readIVectorFNum_vsdlss(0x22,local_44,puVar2);
  printIVectorToLog_vsdlss("From Vec",puVar2,local_44);
  casiFree_vsdlss(puVar2);
  puVar2 = (undefined4 *)newIVector_vsdlss((long)local_44,0,0);
  *puVar2 = 0;
  readIVectorFNum_vsdlss(0x23,local_44,puVar2);
  printIVectorToLog_vsdlss("To Vec",puVar2,local_44);
  casiFree_vsdlss(puVar2);
  puVar5 = (undefined8 *)newVector_vsdlss(0,(long)local_44,0);
  *puVar5 = 0;
  readDVectorFNum_vsdlss(0x24,local_44,puVar5);
  printDVectorToLog_vsdlss("Offdiag Vec",puVar5,local_44);
  casiFree_vsdlss(puVar5);
  printToLog_vsdlss("  End   Reduced Matrix\n");
  printToLog_vsdlss("  --------------------\n\n");
  if (param_3 != 0) {
    puVar5 = (undefined8 *)newVector_vsdlss(0,(long)local_48,0);
    *puVar5 = 0;
    readDVectorFNum_vsdlss(0x25,local_48,puVar5);
    printDVectorToLog_vsdlss("Reduced RHS Vec",puVar5,local_48);
    casiFree_vsdlss(puVar5);
    return;
  }
  pFVar3 = (FILE *)openFileR_vsdlss(0x1f);
  sVar4 = fread(&local_40,4,1,pFVar3);
  if ((int)sVar4 != 1) {
    nrerror_vsdlss(0x10,"logfileutils.c",0x19e,"printPartFactDetailsToLog:nColsParf",1,
                   sVar4 & 0xffffffff);
  }
  printToLog_vsdlss("\n  Begin Partial Factor: Columns %d\n",local_40);
  printToLog_vsdlss("  ----------------------------------------\n");
  puVar2 = (undefined4 *)newIVector_vsdlss((long)(local_40 + local_48),0,0);
  *puVar2 = 0;
  puVar5 = (undefined8 *)newVector_vsdlss(0,(long)(local_40 + local_48),0);
  *puVar5 = 0;
  if (0 < local_40) {
    iVar6 = 1;
    do {
      sVar4 = fread(local_3c,4,1,pFVar3);
      if ((int)sVar4 != 1) {
        nrerror_vsdlss(0x10,"logfileutils.c",0x1a8,"printPartFactDetailsToLog:nz",1,
                       sVar4 & 0xffffffff);
      }
      printToLog_vsdlss("Row %d has %d Non-zeros\n",iVar6,local_3c[0]);
      iVar1 = local_3c[0];
      sVar4 = fread(puVar2 + 1,4,(long)local_3c[0],pFVar3);
      if ((int)sVar4 != iVar1) {
        nrerror_vsdlss(0x10,"logfileutils.c",0x1ac,"printPartFactDetailsToLog:indVec",iVar1,
                       sVar4 & 0xffffffff);
      }
      printIVectorToLog_vsdlss("Ind Vector",puVar2,local_3c[0]);
      iVar1 = local_3c[0];
      sVar4 = fread(puVar5 + 1,8,(long)local_3c[0],pFVar3);
      if ((int)sVar4 != iVar1) {
        nrerror_vsdlss(0x10,"logfileutils.c",0x1b0,"printPartFactDetailsToLog:valVec",iVar1,
                       sVar4 & 0xffffffff);
      }
      iVar6 = iVar6 + 1;
      printDVectorToLog_vsdlss("Val Vector",puVar5,local_3c[0]);
    } while (iVar6 <= local_40);
  }
  printToLog_vsdlss("End   Partial Factor\n");
  printToLog_vsdlss("--------------------\n");
  casiFree_vsdlss(puVar2);
  casiFree_vsdlss(puVar5);
  casiFClose_vsdlss(0x1f);
  return;
}


