/* driver: _yPFrFJBPXSPAIPP addr=00973ee0 size=26 */
#include "vsdlss_ref.h"


void _yPFrFJBPXSPAIPP(clock_t *param_1)

{
  tms local_28;
  
  times(&local_28);
  *param_1 = local_28.tms_utime;
  param_1[1] = local_28.tms_stime;
  return;
}


