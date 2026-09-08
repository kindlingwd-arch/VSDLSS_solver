/* driver: FKFQZMNXSPAIPP addr=009b2210 size=6a */
#include "vsdlss_ref.h"


void FKFQZMNXSPAIPP(undefined8 param_1,undefined8 *param_2)

{
  *param_2 = param_1;
  param_2[1] = param_1;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[4] = 0x7ffffff5;
  param_2[5] = 0x7ffffff5;
  FKFQgtB_QLOXSPAIPP(param_2[6],param_1,0x7ffffff5);
  FKFQgtB_QLOXSPAIPP(param_2[7],param_1,param_2[4]);
  FKFQgtB_QLOXSPAIPP(param_2[8],param_1,param_2[4]);
  return;
}


