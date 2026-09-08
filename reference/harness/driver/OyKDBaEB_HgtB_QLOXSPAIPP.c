/* driver: OyKDBaEB_HgtB_QLOXSPAIPP addr=00962a30 size=7c */
#include "vsdlss_ref.h"


void OyKDBaEB_HgtB_QLOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,undefined8 param_5)

{
  long lVar1;
  long lVar2;
  
  lVar2 = 1;
  if (0 < param_2) {
    do {
      lVar1 = *(long *)(param_1 + lVar2 * 8);
      if ((lVar1 < param_3) || (param_4 < lVar1)) {
        KOBOOLOXSPAIPP(0xb,"utils.c",0x8af,lVar1,lVar2,param_5,param_3,param_4);
      }
      lVar2 = lVar2 + 1;
    } while (lVar2 <= param_2);
  }
  return;
}


