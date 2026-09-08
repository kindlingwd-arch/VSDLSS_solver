/* VSDLSS function (decompiled by Ghidra) */
/* name: readIVecFromFile_vsdlss  addr: 009f5e10  size: 90 */
#include "vsdlss_ref.h"

/* signature: void readIVecFromFile_vsdlss(char *param_1,int param_2,long param_3); */

void readIVecFromFile_vsdlss(char *param_1,int param_2,long param_3)

{
  FILE *__stream;
  size_t sVar1;
  
  __stream = fopen(param_1,"rb");
  if (__stream == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"interface.c",0x20b,param_1,"read");
  }
  sVar1 = fread((void *)(param_3 + 4),4,(long)param_2,__stream);
  if ((int)sVar1 != param_2) {
    nrerror_vsdlss(0x10,"interface.c",0x212,"readIVecFromFile:v",param_2,sVar1 & 0xffffffff);
  }
  fclose(__stream);
  return;
}


