/* driver: OByAaERKHgKCLXSPAIPP addr=009a92b0 size=4c7 */
#include "vsdlss_ref.h"


long OByAaERKHgKCLXSPAIPP(void)

{
  long lVar1;
  FILE *__stream;
  size_t sVar2;
  long lVar3;
  long lVar4;
  long local_60;
  undefined1 local_58 [8];
  undefined8 local_50;
  undefined8 local_48;
  long local_40;
  undefined8 local_38;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(6);
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x278,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(local_58,8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x278,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  lVar4 = 1;
  lVar3 = KBTaERKHgKCLXSPAIPP(local_50,local_48,local_40,local_38);
  if (0 < local_40) {
    do {
      lVar1 = lVar4 * 8;
      lVar4 = lVar4 + 1;
      OByAatB_QLOlLlBTtB_(__stream,*(undefined8 *)(*(long *)(lVar3 + 0x28) + lVar1));
    } while (lVar4 <= local_40);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x284,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x30),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x284,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x285,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x48),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x285,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x286,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x50),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x286,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x287,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x58),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x287,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x288,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x60),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x288,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x289,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x68),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x289,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  sVar2 = fread(&local_60,8,1,__stream);
  if (sVar2 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x28a,"readIVectorNoNewVec:n",1,sVar2);
  }
  sVar2 = fread(*(void **)(lVar3 + 0x70),8,local_60 + 1,__stream);
  if (local_60 + 1U != sVar2) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x28a,"readIVectorNoNewVec:v",local_60 + 1U,sVar2);
  }
  OByAjgtB_QLOlLlBTtB_(__stream,*(undefined8 *)(lVar3 + 0x78));
  _yPFdaILPBXSPAIPP(6);
  return lVar3;
}


