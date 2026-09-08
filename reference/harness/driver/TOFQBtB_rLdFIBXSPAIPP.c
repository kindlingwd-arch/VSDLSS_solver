/* driver: TOFQBtB_rLdFIBXSPAIPP addr=0098aac0 size=92 */
#include "vsdlss_ref.h"


void TOFQBtB_rLdFIBXSPAIPP(char *param_1,size_t param_2,long param_3)

{
  FILE *__s;
  size_t sVar1;
  
  __s = fopen(param_1,"wb");
  if (__s == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"interface.c",0x243,param_1,"write");
  }
  sVar1 = fwrite((void *)(param_3 + 8),8,param_2,__s);
  if (sVar1 != param_2) {
    KOBOOLOXSPAIPP(0x10,"interface.c",0x24a,"writeVecToFile:v",param_2,sVar1);
  }
  fclose(__s);
  return;
}


