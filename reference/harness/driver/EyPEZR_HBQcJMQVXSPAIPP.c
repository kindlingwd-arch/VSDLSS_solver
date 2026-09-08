/* driver: EyPEZR_HBQcJMQVXSPAIPP addr=009b29d0 size=17 */
#include "vsdlss_ref.h"


undefined8 EyPEZR_HBQcJMQVXSPAIPP(long param_1,long *param_2)

{
  return CONCAT71((int7)((ulong)param_2[1] >> 8),
                  *(long *)(param_2[1] + 8 + ((param_1 + -1) % *param_2) * 8) == 0);
}


