/* VSDLSS function (decompiled by Ghidra) */
/* name: readBlockInfo_vsdlss  addr: 00a0f440  size: 368 */
#include "vsdlss_ref.h"

/* signature: long readBlockInfo_vsdlss(void); */

long readBlockInfo_vsdlss(void)

{
  FILE *__stream;
  size_t sVar1;
  long lVar2;
  int local_2c;
  undefined1 local_28 [4];
  undefined4 local_24;
  undefined4 local_20;
  
  __stream = (FILE *)openFileR_vsdlss(1);
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2c6,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(local_28,4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2c6,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  lVar2 = newBlockInfo_vsdlss(local_24,local_20);
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2cc,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 8),4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2cc,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2cd,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x10),4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2cd,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ce,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x18),4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2ce,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  readLIVectorNoNewVec(__stream,*(undefined8 *)(lVar2 + 0x20));
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2d0,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x28),4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2d0,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  sVar1 = fread(&local_2c,4,1,__stream);
  if ((int)sVar1 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2d1,"readIVectorNoNewVec:n",1,sVar1 & 0xffffffff);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x30),4,(long)(local_2c + 1),__stream);
  if (local_2c + 1 != (int)sVar1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x2d1,"readIVectorNoNewVec:v",local_2c + 1,
                   sVar1 & 0xffffffff);
  }
  casiFClose_vsdlss(1);
  return lVar2;
}


