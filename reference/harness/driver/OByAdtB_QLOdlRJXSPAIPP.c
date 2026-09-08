/* driver: OByAdtB_QLOdlRJXSPAIPP addr=00961d00 size=59 */
#include "vsdlss_ref.h"


void OByAdtB_QLOdlRJXSPAIPP(undefined8 param_1,size_t param_2,long param_3)

{
  FILE *__stream;
  size_t sVar1;
  
  __stream = (FILE *)LMBKdFIBpXSPAIPP();
  sVar1 = fread((void *)(param_3 + 8),8,param_2,__stream);
  if (sVar1 != param_2) {
    KOBOOLOXSPAIPP(0x10,"utils.c",0x6c4,"readFVectorFNum:v",param_2,sVar1);
  }
  _yPFdaILPBXSPAIPP(param_1);
  return;
}


