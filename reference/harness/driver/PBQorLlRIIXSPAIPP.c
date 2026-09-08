/* driver: PBQorLlRIIXSPAIPP addr=009a7cc0 size=18 */
#include "vsdlss_ref.h"


undefined8 PBQorLlRIIXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
  }
  return 0;
}


