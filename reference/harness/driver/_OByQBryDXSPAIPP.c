/* driver: _OByQBryDXSPAIPP addr=009900c0 size=d0 */
#include "vsdlss_ref.h"


long _OByQBryDXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = 0;
  do {
    if ((&DAT_00e8be00)[lVar3] == 0) goto LAB_009900e4;
    lVar3 = lVar3 + 1;
  } while (lVar3 != 0x100);
  KOBOOLOXSPAIPP(10,"memmanager.c",0x177);
LAB_009900e4:
  lVar1 = BK_yMPRIyQBAkyIIL_XSPAIPP(0x100000);
  (&DAT_00e8be00)[lVar3] = lVar1;
  if (lVar1 == 0) {
    KOBOOLOXSPAIPP(4,"memmanager.c",0x17f,"encapsulatedMalloc");
    lVar1 = (&DAT_00e8be00)[lVar3];
  }
  lVar2 = 0;
  do {
    *(undefined8 *)(lVar1 + 8 + lVar2) = 0;
    *(undefined8 *)(lVar1 + lVar2) = 0xffffffffffffffff;
    lVar2 = lVar2 + 0x10;
  } while (lVar2 != 0x100000);
  (&DAT_00e8c600)[lVar3] = 0xffffffffffffffff;
  (&DAT_00e8ce00)[lVar3] = 0xffffffffffffffff;
  (&DAT_00e8d600)[lVar3] = 0;
  (&DAT_00e8de00)[lVar3] = 0;
  return lVar3;
}


