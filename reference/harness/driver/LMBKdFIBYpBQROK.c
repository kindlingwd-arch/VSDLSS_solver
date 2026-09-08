/* driver: LMBKdFIBYpBQROK addr=009c80f0 size=5c */
#include "vsdlss_ref.h"


void LMBKdFIBYpBQROK(long param_1)

{
  FILE *pFVar1;
  char local_18 [16];
  
  if (*(long *)(&DAT_00e3b6b8 + param_1 * 0x28) == 0) {
    local_18[0] = 'a';
    local_18[1] = '\0';
  }
  else if (*(long *)(&DAT_00e3b6b8 + param_1 * 0x28) == 1) {
    local_18[2] = 0;
    local_18[0] = 'a';
    local_18[1] = 'b';
  }
  pFVar1 = fopen(*(char **)(&DAT_00e3b6a0 + param_1 * 0x28),local_18);
  *(FILE **)(&DAT_00e3b6c0 + param_1 * 0x28) = pFVar1;
  return;
}


