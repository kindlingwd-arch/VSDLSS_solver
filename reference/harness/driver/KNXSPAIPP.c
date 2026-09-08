/* driver: KNXSPAIPP addr=009a7d50 size=92 */
#include "vsdlss_ref.h"


void KNXSPAIPP(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  
  lVar1 = param_1[1];
  lVar2 = *param_1;
  if (lVar2 == lVar1 + 1) {
    KOBOOLOXSPAIPP(0x35,"queueutils.c",0xa3,"queue");
    lVar2 = *param_1;
    lVar1 = param_1[1];
  }
  if ((lVar2 == 0) && (param_1[2] == lVar1)) {
    KOBOOLOXSPAIPP(0x35,"queueutils.c",0xa8,"queue");
    lVar1 = param_1[1];
  }
  *(undefined8 *)(param_1[3] + lVar1 * 8) = param_2;
  lVar2 = param_1[1];
  param_1[1] = lVar2 + 1;
  if (param_1[2] < lVar2 + 1) {
    param_1[1] = 0;
  }
  return;
}


