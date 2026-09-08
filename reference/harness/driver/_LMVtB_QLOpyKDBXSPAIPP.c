/* driver: _LMVtB_QLOpyKDBXSPAIPP addr=009603f0 size=25 */
#include "vsdlss_ref.h"


void _LMVtB_QLOpyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  if (param_1 <= param_2) {
    do {
      *(undefined8 *)(param_4 + param_1 * 8) = *(undefined8 *)(param_3 + param_1 * 8);
      param_1 = param_1 + 1;
    } while (param_1 != param_2 + 1);
  }
  return;
}


