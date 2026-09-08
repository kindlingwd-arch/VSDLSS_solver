/* driver: _ILPBdy_QLOaERKHdFIBXSPAIPP addr=009749e0 size=31 */
#include "vsdlss_ref.h"


void _ILPBdy_QLOaERKHdFIBXSPAIPP(long param_1,long param_2)

{
  FILE *__stream;
  
  __stream = *(FILE **)(*(long *)(param_1 + 0x38) + param_2 * 8);
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
    *(undefined8 *)(*(long *)(param_1 + 0x38) + param_2 * 8) = 0;
  }
  return;
}


