/* driver: TOFQBbBD1bBD2nCXSPAIPP addr=00975c50 size=1c8 */
#include "vsdlss_ref.h"


void TOFQBbBD1bBD2nCXSPAIPP(undefined8 *param_1)

{
  FILE *__s;
  size_t sVar1;
  long lVar2;
  long lVar3;
  size_t sVar4;
  undefined8 local_40;
  long local_38;
  long local_30 [2];
  
  local_30[0] = param_1[2];
  local_38 = param_1[1];
  local_40 = *param_1;
  lVar3 = local_38 + local_30[0];
  lVar2 = local_30[0] + lVar3;
  __s = (FILE *)LMBKdFIBuXSPAIPP(0xf);
  sVar1 = fwrite(&local_40,8,1,__s);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x10c,"writeDeg1Deg2Pf:nVars",1,sVar1);
  }
  sVar1 = fwrite(&local_38,8,1,__s);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x113,"writeDeg1Deg2Pf:nDeg1Nodes",1,sVar1);
  }
  sVar1 = fwrite(local_30,8,1,__s);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x11a,"writeDeg1Deg2Pf:nDeg2Nodes",1,sVar1);
  }
  sVar4 = lVar3 + 1;
  sVar1 = fwrite((void *)param_1[3],8,sVar4,__s);
  if (sVar1 != sVar4) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x121,"writeDeg1Deg2Pf:pfDiagonalVec(pf)",sVar4,sVar1);
  }
  sVar4 = lVar2 + 1;
  sVar1 = fwrite((void *)param_1[4],8,sVar4,__s);
  if (sVar1 != sVar4) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x128,"writeDeg1Deg2Pf:pfIndex1And2Vec(pf)",sVar4,sVar1);
  }
  sVar1 = fwrite((void *)param_1[5],8,sVar4,__s);
  if (sVar4 != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x12f,"writeDeg1Deg2Pf:pfValue1And2Vec(pf)",sVar4,sVar1);
  }
  _yPFdaILPBXSPAIPP(0xf);
  return;
}


