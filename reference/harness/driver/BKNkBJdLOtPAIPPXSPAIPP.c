/* driver: BKNkBJdLOtPAIPPXSPAIPP addr=0097a040 size=4d */
#include "vsdlss_ref.h"


undefined8 BKNkBJdLOtPAIPPXSPAIPP(void)

{
  FILE *__stream;
  undefined8 local_10;
  
  local_10 = 0;
  __stream = fopen("sdsmem.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%lg",&local_10);
    fclose(__stream);
  }
  return local_10;
}


