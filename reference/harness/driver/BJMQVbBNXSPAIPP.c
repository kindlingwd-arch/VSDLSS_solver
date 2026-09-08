/* driver: BJMQVbBNXSPAIPP addr=009b27e0 size=f */
#include "vsdlss_ref.h"


bool BJMQVbBNXSPAIPP(long param_1)

{
  return *(long *)(param_1 + 8) == *(long *)(param_1 + 0x10);
}


