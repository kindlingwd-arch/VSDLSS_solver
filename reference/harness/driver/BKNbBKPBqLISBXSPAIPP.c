/* driver: BKNbBKPBqLISBXSPAIPP addr=00979f10 size=4c */
#include "vsdlss_ref.h"


undefined8 BKNbBKPBqLISBXSPAIPP(void)

{
  FILE *__stream;
  undefined8 local_10;
  
  local_10 = 0;
  __stream = fopen("dsolve.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    fclose(__stream);
  }
  return local_10;
}


