/* driver: DBQlRJhLzPgKhOQXSPAIPP addr=0098acb0 size=2a */
#include "vsdlss_ref.h"


long DBQlRJhLzPgKhOQXSPAIPP(void)

{
  long lVar1;
  long *plVar2;
  
  plVar2 = &DAT_00e795c0;
  lVar1 = 0;
  do {
    lVar1 = (lVar1 + 1) - (ulong)(*plVar2 == 0);
    plVar2 = plVar2 + 0x94;
  } while (plVar2 != (long *)&DAT_00e8bdc0);
  return lVar1;
}


