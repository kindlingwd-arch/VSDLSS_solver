/* driver: LMBKdFIBYpBQROKXSPAIPP addr=009870d0 size=5c */
#include "vsdlss_ref.h"


void LMBKdFIBYpBQROKXSPAIPP(long param_1)

{
  FILE *pFVar1;
  char local_18 [16];
  
  if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 0) {
    local_18[0] = 'a';
    local_18[1] = '\0';
  }
  else if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 1) {
    local_18[2] = 0;
    local_18[0] = 'a';
    local_18[1] = 'b';
  }
  pFVar1 = fopen(*(char **)(&DAT_00e3ab40 + param_1 * 0x28),local_18);
  *(FILE **)(&DAT_00e3ab60 + param_1 * 0x28) = pFVar1;
  return;
}


