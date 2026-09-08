/* driver: kjbnRPEaLJMLKBKQPXSPAIPP addr=009978b0 size=16d */
#include "vsdlss_ref.h"


void kjbnRPEaLJMLKBKQPXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,undefined8 param_5,
               undefined8 param_6,undefined8 param_7)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 local_48;
  
  if (param_1 < 1) {
    local_48 = 0;
    lVar3 = 1;
  }
  else {
    lVar3 = *(long *)(param_3 + 8);
    if (param_1 == 1) {
      local_48 = 8;
      lVar3 = 2;
    }
    else {
      lVar2 = 2;
      local_48 = 1;
      do {
        lVar1 = *(long *)(param_3 + lVar2 * 8);
        if (lVar3 < lVar1) {
          lVar3 = lVar1;
          local_48 = lVar2;
        }
        lVar2 = lVar2 + 1;
      } while (lVar2 != param_1 + 1);
      if (local_48 == 1) {
        local_48 = 8;
        lVar3 = 2;
      }
      else {
        lVar2 = 1;
        do {
          lVar3 = lVar2;
          MRPEYqQy_HXSPAIPP(param_5,*(undefined8 *)(param_2 + lVar3 * 8));
          MRPEYqQy_HXSPAIPP(param_6,*(undefined8 *)(param_3 + lVar3 * 8));
          lVar2 = lVar3 + 1;
          MRPEYqQy_HXSPAIPP(param_7,*(undefined8 *)(param_4 + lVar3 * 8));
        } while (lVar2 != local_48);
        local_48 = lVar2 * 8;
        lVar3 = lVar3 + 2;
      }
    }
  }
  for (; lVar3 <= param_1; lVar3 = lVar3 + 1) {
    MRPEYqQy_HXSPAIPP(param_5,*(undefined8 *)(param_2 + lVar3 * 8));
    MRPEYqQy_HXSPAIPP(param_6,*(undefined8 *)(param_3 + lVar3 * 8));
    MRPEYqQy_HXSPAIPP(param_7,*(undefined8 *)(param_4 + lVar3 * 8));
  }
  MRPEYqQy_HXSPAIPP(param_5,*(undefined8 *)(param_2 + local_48));
  MRPEYqQy_HXSPAIPP(param_6,*(undefined8 *)(param_3 + local_48));
  MRPEYqQy_HXSPAIPP(param_7,*(undefined8 *)(param_4 + local_48));
  return;
}


