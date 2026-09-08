/* driver: ILLHdLOdFQgKjyPQqQAZIL_HXSPAIPP addr=0098fcd0 size=3f */
#include "vsdlss_ref.h"


long ILLHdLOdFQgKjyPQqQAZIL_HXSPAIPP(long param_1,ulong param_2,ulong param_3)

{
  ulong uVar1;
  long lVar2;
  
  lVar2 = (&DAT_00e8ce00)[param_1];
  if ((-1 < lVar2) &&
     (uVar1 = *(ulong *)(lVar2 * 0x10 + (&DAT_00e8be00)[param_1]), uVar1 - uVar1 % param_3 < param_2
     )) {
    lVar2 = -1;
  }
  return lVar2;
}


