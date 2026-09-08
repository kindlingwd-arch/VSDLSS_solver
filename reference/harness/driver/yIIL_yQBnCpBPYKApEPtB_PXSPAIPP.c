/* driver: yIIL_yQBnCpBPYKApEPtB_PXSPAIPP addr=009aa8c0 size=70 */
#include "vsdlss_ref.h"


void yIIL_yQBnCpBPYKApEPtB_PXSPAIPP(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar1 = *DAT_00e8e750;
  lVar2 = DAT_00e8e750[1];
  lVar3 = DAT_00e8e750[2];
  lVar4 = *(long *)(DAT_00e8e748 + 8);
  DAT_00e8e740 = KBTtB_QLOXSPAIPP(0,lVar2 + lVar3,0);
  DAT_00e8e738 = KBTtB_QLOXSPAIPP(0,lVar4,0);
  DAT_00e8e730 = KBTtB_QLOXSPAIPP(0,lVar1 - (lVar2 + lVar3 + lVar4),0);
  DAT_00e8e728 = DAT_00e8e730;
  return;
}


