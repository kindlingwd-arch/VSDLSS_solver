/* driver: ANnNXSPAIPP addr=009a6d40 size=74 */
#include "vsdlss_ref.h"


long ANnNXSPAIPP(long param_1,undefined8 *param_2)

{
  long lVar1;
  
  if (*(long *)(param_1 + 0x10) < 1) {
    KOBOOLOXSPAIPP(0x26,"priorityqutils.c",0x1cd);
  }
  lVar1 = *(long *)(*(long *)(param_1 + 0x20) + 8);
  *param_2 = *(undefined8 *)(*(long *)(param_1 + 0x28) + lVar1 * 8);
  MNqTyMXSPAIPP(1,*(undefined8 *)(param_1 + 0x10),param_1);
  *(long *)(param_1 + 0x10) = *(long *)(param_1 + 0x10) + -1;
  MNgK_OByPBAiBVgQBOXSPAIPP(1,param_1);
  *(undefined8 *)(*(long *)(param_1 + 0x18) + lVar1 * 8) = 0xffffffffffffffff;
  return lVar1;
}


