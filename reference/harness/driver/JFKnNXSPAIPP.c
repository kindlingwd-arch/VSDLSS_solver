/* driver: JFKnNXSPAIPP addr=009a6d10 size=26 */
#include "vsdlss_ref.h"


void JFKnNXSPAIPP(long param_1,ulong *param_2)

{
  if (*(long *)(param_1 + 0x10) == 0) {
    *param_2 = ~*(ulong *)(param_1 + 8);
    return;
  }
  *param_2 = *(ulong *)(*(long *)(param_1 + 0x28) + *(long *)(*(long *)(param_1 + 0x20) + 8) * 8);
  return;
}


