/* driver: LMBKdFIBuXSPAIPP addr=00986e60 size=b7 */
#include "vsdlss_ref.h"


FILE * LMBKdFIBuXSPAIPP(long param_1)

{
  char *__filename;
  FILE *pFVar1;
  char local_28 [16];
  
  if (*(FILE **)(&DAT_00e3ab60 + param_1 * 0x28) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3ab60 + param_1 * 0x28));
    *(undefined8 *)(&DAT_00e3ab60 + param_1 * 0x28) = 0;
  }
  __filename = *(char **)(&DAT_00e3ab40 + param_1 * 0x28);
  if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 0) {
    local_28[0] = 'w';
    local_28[1] = '\0';
  }
  else if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 1) {
    local_28[2] = 0;
    local_28[0] = 'w';
    local_28[1] = 'b';
  }
  pFVar1 = fopen(__filename,local_28);
  if (pFVar1 == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"fmanager.c",0xd3,__filename,"write");
  }
  *(FILE **)(&DAT_00e3ab60 + param_1 * 0x28) = pFVar1;
  return pFVar1;
}


