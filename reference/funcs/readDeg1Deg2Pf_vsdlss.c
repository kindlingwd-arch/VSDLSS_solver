/* VSDLSS function (decompiled by Ghidra) */
/* name: readDeg1Deg2Pf_vsdlss  addr: 009e3ae0  size: 1ef */
#include "vsdlss_ref.h"

/* signature: long readDeg1Deg2Pf_vsdlss(int param_1); */

long readDeg1Deg2Pf_vsdlss(int param_1)

{
  FILE *__stream;
  size_t sVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c [3];
  
  __stream = (FILE *)openFileR_vsdlss(0xf);
  sVar1 = fread(&local_34,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x14c,"readDeg1Deg2Pf:nVars",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_30,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x153,"readDeg1Deg2Pf:nDeg1Nodes",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x15a,"readDeg1Deg2Pf:nDeg2Nodes",1,sVar1 & 0xffffffff);
  }
  if (param_1 < 0) {
    lVar2 = newDeg1Deg2Pf_vsdlss(local_34,local_30,local_2c[0]);
  }
  else {
    lVar2 = tagNewDeg1Deg2Pf_vsdlss(param_1,local_34,local_30,local_2c[0]);
  }
  iVar3 = *(int *)(lVar2 + 8);
  iVar4 = *(int *)(lVar2 + 4) + iVar3;
  iVar5 = iVar4 + 1;
  sVar1 = fread(*(void **)(lVar2 + 0x10),8,(long)iVar5,__stream);
  if ((int)sVar1 != iVar5) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x16b,"readDeg1Deg2Pf:pfDiagonalVec(pf)",iVar5,
                   sVar1 & 0xffffffff);
  }
  iVar3 = iVar3 + iVar4 + 1;
  sVar1 = fread(*(void **)(lVar2 + 0x18),4,(long)iVar3,__stream);
  if ((int)sVar1 != iVar3) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x172,"readDeg1Deg2Pf:pfIndex1And2Vec(pf)",iVar3,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x20),8,(long)iVar3,__stream);
  if (iVar3 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x179,"readDeg1Deg2Pf:pfValue1And2Vec(pf)",iVar3,
                   sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(0xf);
  return lVar2;
}


