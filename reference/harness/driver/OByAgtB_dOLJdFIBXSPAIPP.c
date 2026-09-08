/* driver: OByAgtB_dOLJdFIBXSPAIPP addr=0098a980 size=92 */
#include "vsdlss_ref.h"


void OByAgtB_dOLJdFIBXSPAIPP(char *param_1,size_t param_2,long param_3)

{
  FILE *__stream;
  size_t sVar1;
  
  __stream = fopen(param_1,"rb");
  if (__stream == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"interface.c",0x20b,param_1,"read");
  }
  sVar1 = fread((void *)(param_3 + 8),8,param_2,__stream);
  if (sVar1 != param_2) {
    KOBOOLOXSPAIPP(0x10,"interface.c",0x212,"readIVecFromFile:v",param_2,sVar1);
  }
  fclose(__stream);
  return;
}


