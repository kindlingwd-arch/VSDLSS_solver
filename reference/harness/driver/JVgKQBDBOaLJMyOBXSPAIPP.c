/* driver: JVgKQBDBOaLJMyOBXSPAIPP addr=009600f0 size=14 */
#include "vsdlss_ref.h"


ulong JVgKQBDBOaLJMyOBXSPAIPP(long *param_1,long *param_2)

{
  ulong uVar1;
  
  uVar1 = 0xffffffff;
  if (*param_2 <= *param_1) {
    uVar1 = (ulong)(*param_1 != *param_2);
  }
  return uVar1;
}


