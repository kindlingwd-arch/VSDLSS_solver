/* driver: BKNmOABOmKIVXSPAIPP addr=0097a090 size=4c */
#include "vsdlss_ref.h"


undefined8 BKNmOABOmKIVXSPAIPP(void)

{
  FILE *__stream;
  undefined8 local_10;
  
  local_10 = 0;
  __stream = fopen("orderonly.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    fclose(__stream);
  }
  return local_10;
}


