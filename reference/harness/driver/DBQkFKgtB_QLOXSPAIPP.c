/* driver: DBQkFKgtB_QLOXSPAIPP addr=00961250 size=4d */
#include "vsdlss_ref.h"


long DBQkFKgtB_QLOXSPAIPP(long param_1,long param_2,long *param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if (0 < param_2) {
    lVar2 = *(long *)(param_1 + 8);
    if (param_2 == 1) {
      lVar4 = 1;
    }
    else {
      lVar3 = 2;
      lVar4 = 1;
      do {
        lVar1 = *(long *)(param_1 + lVar3 * 8);
        if (lVar1 < lVar2) {
          lVar2 = lVar1;
          lVar4 = lVar3;
        }
        lVar3 = lVar3 + 1;
      } while (lVar3 != param_2 + 1);
    }
    *param_3 = lVar4;
    return lVar2;
  }
  *param_3 = 0;
  return 0;
}


