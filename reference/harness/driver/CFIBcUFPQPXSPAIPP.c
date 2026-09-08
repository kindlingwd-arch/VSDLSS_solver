/* driver: CFIBcUFPQPXSPAIPP addr=009872b0 size=61 */
#include "vsdlss_ref.h"


bool CFIBcUFPQPXSPAIPP(long param_1)

{
  FILE *__stream;
  char local_18 [24];
  
  if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 0) {
    local_18[0] = 'r';
    local_18[1] = '\0';
  }
  else if (*(long *)(&DAT_00e3ab58 + param_1 * 0x28) == 1) {
    local_18[2] = 0;
    local_18[0] = 'r';
    local_18[1] = 'b';
  }
  __stream = fopen(*(char **)(&DAT_00e3ab40 + param_1 * 0x28),local_18);
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  return __stream != (FILE *)0x0;
}


