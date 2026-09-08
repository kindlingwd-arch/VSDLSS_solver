/* VSDLSS function (decompiled by Ghidra) */
/* name: writeVecToFile_vsdlss  addr: 009f5f50  size: 90 */
#include "vsdlss_ref.h"

/* signature: void writeVecToFile_vsdlss(char *param_1,int param_2,long param_3); */

void writeVecToFile_vsdlss(char *param_1,int param_2,long param_3)

{
  FILE *__s;
  size_t sVar1;
  
  __s = fopen(param_1,"wb");
  if (__s == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"interface.c",0x243,param_1,"write");
  }
  sVar1 = fwrite((void *)(param_3 + 8),8,(long)param_2,__s);
  if ((int)sVar1 != param_2) {
    nrerror_vsdlss(0x10,"interface.c",0x24a,"writeVecToFile:v",param_2,sVar1 & 0xffffffff);
  }
  fclose(__s);
  return;
}


