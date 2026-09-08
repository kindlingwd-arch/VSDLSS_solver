/* driver: FKFQmOABOqBIB_QLOXSPAIPP addr=0097a0e0 size=62 */
#include "vsdlss_ref.h"


void FKFQmOABOqBIB_QLOXSPAIPP(ulong param_1)

{
  FILE *__stream;
  ulong local_10;
  
  local_10 = param_1;
  __stream = fopen("orderselector.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,"%ld",&local_10);
    if (2 < local_10) {
      local_10 = 0;
    }
    fclose(__stream);
  }
  DAT_00e74f48 = local_10;
  return;
}


