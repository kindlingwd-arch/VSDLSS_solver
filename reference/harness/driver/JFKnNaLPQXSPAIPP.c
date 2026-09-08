/* driver: JFKnNaLPQXSPAIPP addr=009a6a10 size=20 */
#include "vsdlss_ref.h"


long JFKnNaLPQXSPAIPP(long param_1,long param_2,long param_3)

{
  if (*(long *)(*(long *)(param_3 + 0x28) + *(long *)(*(long *)(param_3 + 0x20) + param_2 * 8) * 8)
      < *(long *)(*(long *)(param_3 + 0x28) + *(long *)(*(long *)(param_3 + 0x20) + param_1 * 8) * 8
                 )) {
    param_1 = param_2;
  }
  return param_1;
}


