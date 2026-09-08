/* driver: QyDlBTbBD1bBD2nCXSPAIPP addr=009757f0 size=38 */
#include "vsdlss_ref.h"


long QyDlBTbBD1bBD2nCXSPAIPP(undefined8 param_1)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = QyDlBTbBD1bBD2nClLbFyDtB_XSPAIPP();
  uVar2 = QyDlBTdtB_QLOXSPAIPP(0,param_1,*(long *)(lVar1 + 8) + *(long *)(lVar1 + 0x10),1);
  *(undefined8 *)(lVar1 + 0x18) = uVar2;
  return lVar1;
}


