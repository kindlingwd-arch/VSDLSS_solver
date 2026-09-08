/* driver: BKNeBKkyQqQyQPXSPAIPP addr=00979f60 size=5f */
#include "vsdlss_ref.h"


ulong BKNeBKkyQqQyQPXSPAIPP(void)

{
  FILE *__stream;
  ulong local_10;
  
  local_10 = 0;
  __stream = fopen("genmatstats.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    if (4 < local_10) {
      local_10 = 0;
    }
    fclose(__stream);
  }
  return local_10;
}


