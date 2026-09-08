/* driver: P_yIBrFJBXSPAIPP addr=009601a0 size=25 */
#include "vsdlss_ref.h"


void P_yIBrFJBXSPAIPP(double param_1,long *param_2,long *param_3)

{
  long lVar1;
  
  lVar1 = param_2[1];
  *param_3 = (long)((double)*param_2 * param_1);
  param_3[1] = (long)((double)lVar1 * param_1);
  return;
}


