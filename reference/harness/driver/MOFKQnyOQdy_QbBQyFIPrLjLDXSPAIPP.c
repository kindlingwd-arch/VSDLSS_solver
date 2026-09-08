/* driver: MOFKQnyOQdy_QbBQyFIPrLjLDXSPAIPP addr=0098dad0 size=4c7 */
#include "vsdlss_ref.h"


void MOFKQnyOQdy_QbBQyFIPrLjLDXSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  undefined8 *puVar1;
  FILE *pFVar2;
  size_t sVar3;
  undefined8 *puVar4;
  size_t sVar5;
  long lVar6;
  long local_58;
  undefined8 local_50;
  long local_48;
  size_t local_40 [2];
  
  puVar1 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_1,0,0);
  *puVar1 = 0;
  OByAgtB_QLOdlRJXSPAIPP(0x26,param_1,puVar1);
  MOFKQgtB_QLOrLjLDXSPAIPP("Permutation Vector",puVar1,param_1);
  _yPFdOBBXSPAIPP(puVar1);
  pFVar2 = (FILE *)LMBKdFIBpXSPAIPP(0x20);
  sVar3 = fread(&local_58,8,1,pFVar2);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x167,"printPartFactDetailsToLog:WSize",1,sVar3);
  }
  if (local_58 != param_2) {
    KOBOOLOXSPAIPP(1,"logfileutils.c",0x169);
  }
  sVar3 = fread(&local_50,8,1,pFVar2);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x16e,"printPartFactDetailsToLog:nOffdiagsRed",1,sVar3);
  }
  _yPFdaILPBXSPAIPP(0x20);
  MOFKQrLjLDXSPAIPP("\n  Begin Reduced Matrix: Rows %ld ; Number of Offdiags: %ld\n",local_58,
                    local_50);
  MOFKQrLjLDXSPAIPP("  ----------------------------------------------\n");
  puVar1 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_58,0);
  *puVar1 = 0;
  OByAbtB_QLOdlRJXSPAIPP(0x21,local_58,puVar1);
  MOFKQbtB_QLOrLjLDXSPAIPP("Diagonal Vec",puVar1,local_58);
  _yPFdOBBXSPAIPP(puVar1);
  puVar1 = (undefined8 *)KBTgtB_QLOXSPAIPP(local_50,0,0);
  *puVar1 = 0;
  OByAgtB_QLOdlRJXSPAIPP(0x22,local_50,puVar1);
  MOFKQgtB_QLOrLjLDXSPAIPP("From Vec",puVar1,local_50);
  _yPFdOBBXSPAIPP(puVar1);
  puVar1 = (undefined8 *)KBTgtB_QLOXSPAIPP(local_50,0,0);
  *puVar1 = 0;
  OByAgtB_QLOdlRJXSPAIPP(0x23,local_50,puVar1);
  MOFKQgtB_QLOrLjLDXSPAIPP("To Vec",puVar1,local_50);
  _yPFdOBBXSPAIPP(puVar1);
  puVar1 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_50,0);
  *puVar1 = 0;
  OByAbtB_QLOdlRJXSPAIPP(0x24,local_50,puVar1);
  MOFKQbtB_QLOrLjLDXSPAIPP("Offdiag Vec",puVar1,local_50);
  _yPFdOBBXSPAIPP(puVar1);
  MOFKQrLjLDXSPAIPP("  End   Reduced Matrix\n");
  MOFKQrLjLDXSPAIPP("  --------------------\n\n");
  if (param_3 != 0) {
    puVar1 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_58,0);
    *puVar1 = 0;
    OByAbtB_QLOdlRJXSPAIPP(0x25,local_58,puVar1);
    MOFKQbtB_QLOrLjLDXSPAIPP("Reduced RHS Vec",puVar1,local_58);
    _yPFdOBBXSPAIPP(puVar1);
    return;
  }
  pFVar2 = (FILE *)LMBKdFIBpXSPAIPP(0x1f);
  sVar3 = fread(&local_48,8,1,pFVar2);
  if (sVar3 != 1) {
    KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x19e,"printPartFactDetailsToLog:nColsParf",1,sVar3);
  }
  MOFKQrLjLDXSPAIPP("\n  Begin Partial Factor: Columns %ld\n",local_48);
  MOFKQrLjLDXSPAIPP("  ----------------------------------------\n");
  puVar1 = (undefined8 *)KBTgtB_QLOXSPAIPP(local_48 + local_58,0,0);
  *puVar1 = 0;
  puVar4 = (undefined8 *)KBTtB_QLOXSPAIPP(0,local_48 + local_58,0);
  *puVar4 = 0;
  if (0 < local_48) {
    lVar6 = 1;
    do {
      sVar3 = fread(local_40,8,1,pFVar2);
      if (sVar3 != 1) {
        KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x1a8,"printPartFactDetailsToLog:nz",1,sVar3);
      }
      MOFKQrLjLDXSPAIPP("Row %ld has %ld Non-zeros\n",lVar6,local_40[0]);
      sVar3 = local_40[0];
      sVar5 = fread(puVar1 + 1,8,local_40[0],pFVar2);
      if (sVar5 != sVar3) {
        KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x1ac,"printPartFactDetailsToLog:indVec",sVar3,sVar5);
      }
      MOFKQgtB_QLOrLjLDXSPAIPP("Ind Vector",puVar1,local_40[0]);
      sVar3 = local_40[0];
      sVar5 = fread(puVar4 + 1,8,local_40[0],pFVar2);
      if (sVar5 != sVar3) {
        KOBOOLOXSPAIPP(0x10,"logfileutils.c",0x1b0,"printPartFactDetailsToLog:valVec",sVar3,sVar5);
      }
      lVar6 = lVar6 + 1;
      MOFKQbtB_QLOrLjLDXSPAIPP("Val Vector",puVar4,local_40[0]);
    } while (lVar6 <= local_48);
  }
  MOFKQrLjLDXSPAIPP("End   Partial Factor\n");
  MOFKQrLjLDXSPAIPP("--------------------\n");
  _yPFdOBBXSPAIPP(puVar1);
  _yPFdOBBXSPAIPP(puVar4);
  _yPFdaILPBXSPAIPP(0x1f);
  return;
}


