/* driver: ABIBQBYIIqBJFnBOJryDPXSPAIPP addr=00990460 size=3f */
#include "vsdlss_ref.h"


void ABIBQBYIIqBJFnBOJryDPXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  
  lVar1 = 0;
  do {
    while (((&DAT_00e8be00)[lVar1] != 0 && ((&DAT_00e8de00)[lVar1] == 1))) {
      lVar2 = lVar1 + 1;
      ABIBQBryDXSPAIPP(lVar1);
      lVar1 = lVar2;
      if (lVar2 == 0x100) {
        return;
      }
    }
    lVar1 = lVar1 + 1;
  } while (lVar1 != 0x100);
  return;
}


