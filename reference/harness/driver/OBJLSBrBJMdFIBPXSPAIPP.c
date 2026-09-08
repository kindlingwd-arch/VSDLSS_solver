/* driver: OBJLSBrBJMdFIBPXSPAIPP addr=00987210 size=49 */
#include "vsdlss_ref.h"


void OBJLSBrBJMdFIBPXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  
  plVar3 = &DAT_00e3ab78;
  lVar1 = 1;
  do {
    while (*plVar3 == 2) {
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


