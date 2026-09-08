/* driver: _yPFdaILPBXSPAIPP addr=00987140 size=27 */
#include "vsdlss_ref.h"


void _yPFdaILPBXSPAIPP(long param_1)

{
  if (*(FILE **)(&DAT_00e3ab60 + param_1 * 0x28) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3ab60 + param_1 * 0x28));
    *(undefined8 *)(&DAT_00e3ab60 + param_1 * 0x28) = 0;
  }
  return;
}


