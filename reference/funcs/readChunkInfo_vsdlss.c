/* VSDLSS function (decompiled by Ghidra) */
/* name: readChunkInfo_vsdlss  addr: 00a0ebe0  size: 4bc */
#include "vsdlss_ref.h"

/* signature: long readChunkInfo_vsdlss(void); */

long readChunkInfo_vsdlss(void)

{
  undefined8 *puVar1;
  FILE *__stream;
  size_t sVar2;
  long lVar3;
  int iVar4;
  long lVar5;
  int local_4c;
  undefined1 local_48 [4];
  undefined4 local_44;
  undefined4 local_40;
  int local_3c;
  undefined4 local_38;
  
  __stream = (FILE *)openFileR_vsdlss(6);
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x278,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(local_48,4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x278,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  lVar5 = 8;
  iVar4 = 1;
  lVar3 = newChunkInfo_vsdlss(local_44,local_40,local_3c,local_38);
  if (0 < local_3c) {
    do {
      iVar4 = iVar4 + 1;
      puVar1 = (undefined8 *)(*(long *)(lVar3 + 0x18) + lVar5);
      lVar5 = lVar5 + 8;
      readCVectorNoNewVec(__stream,*puVar1);
    } while (iVar4 <= local_3c);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x284,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x20),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x284,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x285,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x38),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x285,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x286,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x40),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x286,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x287,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x48),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x287,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x288,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x50),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x288,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x289,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x58),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x289,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  sVar2 = fread(&local_4c,4,1,__stream);
  if ((int)sVar2 != 1) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x28a,"readIVectorNoNewVec:n",1,sVar2 & 0xffffffff);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x60),4,(long)(local_4c + 1),__stream);
  if (local_4c + 1 != (int)sVar2) {
    nrerror_vsdlss(0x10,"readwriteutils.c",0x28a,"readIVectorNoNewVec:v",local_4c + 1,
                   sVar2 & 0xffffffff);
  }
  readLIVectorNoNewVec(__stream,*(undefined8 *)(lVar3 + 0x68));
  casiFClose_vsdlss(6);
  return lVar3;
}


