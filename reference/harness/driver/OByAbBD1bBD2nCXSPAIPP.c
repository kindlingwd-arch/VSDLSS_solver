/* driver: OByAbBD1bBD2nCXSPAIPP addr=00975e20 size=1f8 */
#include "vsdlss_ref.h"


long OByAbBD1bBD2nCXSPAIPP(long param_1)

{
  long lVar1;
  FILE *__stream;
  size_t sVar2;
  long lVar3;
  long lVar4;
  size_t sVar5;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30 [2];
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(0xf);
  sVar2 = fread(&local_40,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x14c,"readDeg1Deg2Pf:nVars",1,sVar2);
  }
  sVar2 = fread(&local_38,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x153,"readDeg1Deg2Pf:nDeg1Nodes",1,sVar2);
  }
  sVar2 = fread(local_30,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x15a,"readDeg1Deg2Pf:nDeg2Nodes",1,sVar2);
  }
  if (param_1 < 0) {
    lVar3 = KBTbBD1bBD2nCXSPAIPP(local_40,local_38,local_30[0]);
  }
  else {
    lVar3 = QyDlBTbBD1bBD2nCXSPAIPP(param_1,local_40,local_38,local_30[0]);
  }
  lVar1 = *(long *)(lVar3 + 0x10);
  lVar4 = lVar1 + *(long *)(lVar3 + 8);
  sVar5 = lVar4 + 1;
  sVar2 = fread(*(void **)(lVar3 + 0x18),8,sVar5,__stream);
  if (sVar2 != sVar5) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x16b,"readDeg1Deg2Pf:pfDiagonalVec(pf)",sVar5,sVar2);
  }
  sVar5 = lVar1 + lVar4 + 1;
  sVar2 = fread(*(void **)(lVar3 + 0x20),8,sVar5,__stream);
  if (sVar2 != sVar5) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x172,"readDeg1Deg2Pf:pfIndex1And2Vec(pf)",sVar5,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x28),8,sVar5,__stream);
  if (sVar5 != sVar2) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x179,"readDeg1Deg2Pf:pfValue1And2Vec(pf)",sVar5,sVar2);
  }
  _yPFdaILPBXSPAIPP(0xf);
  return lVar3;
}


