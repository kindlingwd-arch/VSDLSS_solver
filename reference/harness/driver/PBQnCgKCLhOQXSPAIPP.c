/* driver: PBQnCgKCLhOQXSPAIPP addr=0098ad80 size=5a */
#include "vsdlss_ref.h"


void PBQnCgKCLhOQXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4
                        )

{
  long lVar1;
  
  lVar1 = CFKAhLzgKhOQpBQROK();
  if (lVar1 == -1) {
    FUN_0098ac50();
  }
  lVar1 = lVar1 * 0x4a0;
  *(undefined8 *)(&DAT_00e79550 + lVar1) = param_4;
  *(undefined8 *)(&DAT_00e79548 + lVar1) = param_3;
  *(undefined8 *)(&DAT_00e79540 + lVar1) = param_2;
  *(undefined8 *)(&DAT_00e79538 + lVar1) = param_1;
  return;
}


