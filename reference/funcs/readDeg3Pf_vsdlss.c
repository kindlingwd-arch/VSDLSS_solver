/* VSDLSS function (decompiled by Ghidra) */
/* name: readDeg3Pf_vsdlss  addr: 009e4190  size: 485 */
#include "vsdlss_ref.h"

/* signature: long readDeg3Pf_vsdlss(int param_1); */

long readDeg3Pf_vsdlss(int param_1)

{
  FILE *__stream;
  size_t sVar1;
  long lVar2;
  int local_6c;
  undefined1 local_68 [4];
  undefined4 local_64;
  undefined4 local_60;
  
  __stream = (FILE *)openFileR_vsdlss(4);
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1b4,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(local_68,4,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1b4,"readIVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  if (param_1 < 0) {
    lVar2 = newDeg3Pf_vsdlss(local_64,local_60);
  }
  else {
    lVar2 = tagNewDeg3Pf_vsdlss(param_1,local_64);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1be,"readFVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 8),8,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1be,"readFVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1bf,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x10),4,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1bf,"readIVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c0,"readFVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x18),8,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c0,"readFVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c1,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x20),4,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c1,"readIVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c2,"readFVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x28),8,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c2,"readFVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c3,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x30),4,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c3,"readIVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_6c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c4,"readFVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x38),8,(long)(local_6c + 1),__stream);
  if (local_6c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"deg123pfutils.c",0x1c4,"readFVectorNoNewVec:v",local_6c + 1,
                   sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(4);
  return lVar2;
}


