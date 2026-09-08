/* driver: FKFQmRQmCaLOBXSPAIPP addr=0097a170 size=62 */
#include "vsdlss_ref.h"


void FKFQmRQmCaLOBXSPAIPP(long param_1)

{
  FILE *__stream;
  long local_10;
  
  local_10 = param_1;
  __stream = fopen("outofcore.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    if (local_10 != 0) {
      local_10 = 1;
    }
    fclose(__stream);
  }
  DAT_00e74f50 = local_10;
  return;
}


