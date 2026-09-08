/* driver: _yPFnRPEXSPAIPP addr=009b2810 size=13 */
#include "vsdlss_ref.h"


void _yPFnRPEXSPAIPP(undefined8 param_1,long param_2)

{
  undefined8 uVar1;
  
  uVar1 = KBTacADBIFPQXSPAIPP(param_1,*(undefined8 *)(param_2 + 8));
  *(undefined8 *)(param_2 + 8) = uVar1;
  return;
}


