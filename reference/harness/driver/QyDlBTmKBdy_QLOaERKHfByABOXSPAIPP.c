/* driver: QyDlBTmKBdy_QLOaERKHfByABOXSPAIPP addr=00986c20 size=9a */
#include "vsdlss_ref.h"


void QyDlBTmKBdy_QLOaERKHfByABOXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)KBTZIL_HXSPAIPP(param_1,0x90);
  *puVar1 = param_1;
  puVar1[1] = 1;
  puVar1[3] = 1;
  puVar1[4] = param_2;
  puVar1[2] = param_2;
  puVar1[6] = 1;
  puVar1[7] = param_2;
  puVar1[5] = param_2;
  puVar1[8] = 0;
  puVar1[9] = 0;
  puVar1[10] = 0;
  puVar1[0xb] = 0;
  puVar1[0xc] = 0;
  puVar1[0xd] = 0;
  puVar1[0xe] = 0;
  puVar1[0xf] = 0;
  puVar1[0x10] = 0;
  puVar1[0x11] = param_2;
  return;
}


