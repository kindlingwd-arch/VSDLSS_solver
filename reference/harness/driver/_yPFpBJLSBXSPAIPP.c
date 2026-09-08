/* driver: _yPFpBJLSBXSPAIPP addr=00987170 size=52 */
#include "vsdlss_ref.h"


void _yPFpBJLSBXSPAIPP(long param_1)

{
  char *__filename;
  long lVar1;
  
  lVar1 = param_1 * 0x28;
  __filename = *(char **)(&DAT_00e3ab40 + lVar1);
  if (*(FILE **)(&DAT_00e3ab60 + lVar1) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3ab60 + lVar1));
    *(undefined8 *)(&DAT_00e3ab60 + lVar1) = 0;
  }
  if (*(long *)(&DAT_00e3ab50 + param_1 * 0x28) != 3) {
    remove(__filename);
    return;
  }
  return;
}


