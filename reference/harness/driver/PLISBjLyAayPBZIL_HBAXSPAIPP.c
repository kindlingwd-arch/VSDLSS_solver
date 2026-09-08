/* driver: PLISBjLyAayPBZIL_HBAXSPAIPP addr=0096dc20 size=aa */
#include "vsdlss_ref.h"


void PLISBjLyAayPBZIL_HBAXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  
  if (*(long *)(param_1 + 0x78) != 0) {
    lVar1 = *(long *)(param_1 + 0x10);
    uVar2 = *(undefined8 *)(param_1 + 0x28);
    CLOTyOAqLISBZIL_HBAXjpyKDB(param_1,param_2,1,uVar2,1,lVar1,lVar1,param_2 + lVar1 * 8);
    lVar3 = *(long *)(param_1 + 0x70);
    lVar4 = 1;
    if (0 < lVar1) {
      do {
        *(double *)(param_2 + lVar4 * 8) =
             *(double *)(param_2 + lVar4 * 8) * *(double *)(lVar3 + lVar4 * 8);
        lVar4 = lVar4 + 1;
      } while (lVar4 != lVar1 + 1);
    }
    zy_HTyOAqLISBZIL_HBAXjpyKDB(param_1,param_2,1,uVar2,lVar1);
    return;
  }
  CLOTyOAqLISBZIL_HBAXSPAIPP();
  zy_HTyOAqLISBZIL_HBAXSPAIPP(param_1,param_2);
  return;
}


