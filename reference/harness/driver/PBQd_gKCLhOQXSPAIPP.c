/* driver: PBQd_gKCLhOQXSPAIPP addr=0098ade0 size=5a */
#include "vsdlss_ref.h"


void PBQd_gKCLhOQXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4
                        )

{
  long lVar1;
  
  lVar1 = CFKAhLzgKhOQpBQROK();
  if (lVar1 == -1) {
    FUN_0098ac50();
  }
  lVar1 = lVar1 * 0x4a0;
  *(undefined8 *)(&DAT_00e79570 + lVar1) = param_4;
  *(undefined8 *)(&DAT_00e79568 + lVar1) = param_3;
  *(undefined8 *)(&DAT_00e79560 + lVar1) = param_2;
  *(undefined8 *)(&DAT_00e79558 + lVar1) = param_1;
  return;
}


