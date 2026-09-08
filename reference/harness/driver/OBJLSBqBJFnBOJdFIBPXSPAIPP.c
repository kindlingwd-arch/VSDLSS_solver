/* driver: OBJLSBqBJFnBOJdFIBPXSPAIPP addr=00987260 size=49 */
#include "vsdlss_ref.h"


void OBJLSBqBJFnBOJdFIBPXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  
  plVar3 = &DAT_00e3ab78;
  lVar1 = 1;
  do {
    while (*plVar3 == 1) {
      lVar2 = lVar1 + 1;
      plVar3 = plVar3 + 5;
      _yPFpBJLSBXSPAIPP(lVar1);
      lVar1 = lVar2;
      if (lVar2 == 0x40) {
        return;
      }
    }
    lVar1 = lVar1 + 1;
    plVar3 = plVar3 + 5;
  } while (lVar1 != 0x40);
  return;
}


