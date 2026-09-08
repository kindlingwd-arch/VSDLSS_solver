/* driver: KBTbBD1bBD2nCXSPAIPP addr=009757c0 size=28 */
#include "vsdlss_ref.h"


long KBTbBD1bBD2nCXSPAIPP(void)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = KBTbBD1bBD2nClLbFyDtB_XSPAIPP();
  uVar2 = KBTdtB_QLOXSPAIPP(0,*(long *)(lVar1 + 8) + *(long *)(lVar1 + 0x10),1);
  *(undefined8 *)(lVar1 + 0x18) = uVar2;
  return lVar1;
}


