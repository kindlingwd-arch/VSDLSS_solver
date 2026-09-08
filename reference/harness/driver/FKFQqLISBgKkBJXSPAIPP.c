/* driver: FKFQqLISBgKkBJXSPAIPP addr=0097a280 size=62 */
#include "vsdlss_ref.h"


void FKFQqLISBgKkBJXSPAIPP(ulong param_1)

{
  FILE *__stream;
  ulong local_10;
  
  local_10 = param_1;
  __stream = fopen("solveinmem.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    if (2 < local_10) {
      local_10 = 0;
    }
    fclose(__stream);
  }
  DAT_00e74f60 = local_10;
  return;
}


