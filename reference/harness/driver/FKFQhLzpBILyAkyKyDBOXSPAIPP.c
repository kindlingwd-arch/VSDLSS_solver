/* driver: FKFQhLzpBILyAkyKyDBOXSPAIPP addr=0098ac70 size=37 */
#include "vsdlss_ref.h"


void FKFQhLzpBILyAkyKyDBOXSPAIPP(void)

{
  undefined8 *puVar1;
  
  if (DAT_00e3b540 != 0) {
    DAT_00e3b540 = 0;
    puVar1 = &DAT_00e795c0;
    do {
      *puVar1 = 0;
      puVar1 = puVar1 + 0x94;
    } while (puVar1 != (undefined8 *)&DAT_00e8bdc0);
  }
  return;
}


