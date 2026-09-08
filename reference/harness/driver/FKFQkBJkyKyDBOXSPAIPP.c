/* driver: FKFQkBJkyKyDBOXSPAIPP addr=00990370 size=e2 */
#include "vsdlss_ref.h"


long FKFQkBJkyKyDBOXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  
  lVar1 = BKNqLISBgKkBJXSPAIPP();
  if ((lVar1 == 0) || ((param_2 != 4 && (param_2 != 2)))) {
    if (DAT_00e3b550 != 0) {
      DAT_00e3b550 = 0;
      param_1 = param_1 * DAT_00e8bde8;
      puVar2 = &DAT_00e8be00;
      DAT_00e8bdf8 = param_1;
      do {
        *puVar2 = 0;
        puVar2[0x100] = 0xffffffffffffffff;
        puVar3 = puVar2 + 1;
        puVar2[0x200] = 0xffffffffffffffff;
        puVar2[0x300] = 0;
        puVar2[0x400] = 0;
        puVar2 = puVar3;
      } while (puVar3 != &DAT_00e8c600);
      return param_1;
    }
    ABIBQBYIIrBJMryDPXSPAIPP();
    ABIBQBhLzdOLJkBJLOVhOQXSPAIPP();
    lVar1 = DBQlRJhLzPgKhOQXSPAIPP();
    if (lVar1 == 0) {
      DAT_00e8bdf8 = param_1 * DAT_00e8bde8;
      return param_1 * DAT_00e8bde8;
    }
  }
  return DAT_00e8bdf8;
}


