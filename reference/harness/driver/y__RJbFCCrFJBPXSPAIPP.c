/* driver: y__RJbFCCrFJBPXSPAIPP addr=00960160 size=16 */
#include "vsdlss_ref.h"


void y__RJbFCCrFJBPXSPAIPP(long *param_1,long *param_2,long *param_3)

{
  *param_3 = *param_3 + (*param_2 - *param_1);
  param_3[1] = param_3[1] + (param_2[1] - param_1[1]);
  return;
}


