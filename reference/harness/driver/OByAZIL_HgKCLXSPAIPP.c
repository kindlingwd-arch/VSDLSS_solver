/* driver: OByAZIL_HgKCLXSPAIPP addr=009a9b30 size=370 */
#include "vsdlss_ref.h"


long OByAZIL_HgKCLXSPAIPP(void)

{
  FILE *__stream;
  size_t sVar1;
  long lVar2;
  long local_40;
  undefined1 local_38 [8];
  undefined8 local_30;
  undefined8 local_28;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP(1);
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2c6,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(local_38,8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2c6,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  lVar2 = KBTZIL_HgKCLXSPAIPP(local_30,local_28);
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2cc,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x10),8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2cc,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2cd,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x18),8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2cd,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ce,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x20),8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2ce,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  OByAjgtB_QLOlLlBTtB_(__stream,*(undefined8 *)(lVar2 + 0x28));
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2d0,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x30),8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2d0,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  sVar1 = fread(&local_40,8,1,__stream);
  if (sVar1 != 1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2d1,"readIVectorNoNewVec:n",1,sVar1);
  }
  sVar1 = fread(*(void **)(lVar2 + 0x38),8,local_40 + 1,__stream);
  if (local_40 + 1U != sVar1) {
    KOBOOLOXSPAIPP(0x10,"readwriteutils.c",0x2d1,"readIVectorNoNewVec:v",local_40 + 1U,sVar1);
  }
  _yPFdaILPBXSPAIPP(1);
  return lVar2;
}


