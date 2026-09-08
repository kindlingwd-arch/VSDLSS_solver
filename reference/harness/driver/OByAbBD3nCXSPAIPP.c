/* driver: OByAbBD3nCXSPAIPP addr=009764e0 size=49a */
#include "vsdlss_ref.h"


long OByAbBD3nCXSPAIPP(long param_1)

{
  FILE *__stream;
  size_t sVar1;
  long lVar2;
  long local_b0;
  undefined1 local_a8 [8];
  undefined8 local_a0;
  undefined8 local_98;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(4);
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1b4,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(local_a8,8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1b4,"readIVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  if (param_1 < 0) {
    lVar2 = KBTbBD3nCXSPAIPP(local_a0,local_98);
  }
  else {
    lVar2 = QyDlBTbBD3nCXSPAIPP(param_1,local_a0);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1be,"readFVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x10),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1be,"readFVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1bf,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x18),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1bf,"readIVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c0,"readFVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x20),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c0,"readFVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c1,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x28),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c1,"readIVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c2,"readFVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x30),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c2,"readFVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c3,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x38),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c3,"readIVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  sVar1 = fread(&local_b0,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c4,"readFVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x40),8,local_b0 + 1,__stream);
  if (local_b0 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"deg123pfutils.c",0x1c4,"readFVectorNoNewVec:v",local_b0 + 1U,sVar1);
  }
  _yPFdaILPBXSPAIPP(4);
  return lVar2;
}


