/* driver: DBQqByO_EgKAF_BPqLOQBAXSPAIPP addr=0097b010 size=ca */
#include "vsdlss_ref.h"


void DBQqByO_EgKAF_BPqLOQBAXSPAIPP(long param_1,long param_2,long param_3,long param_4,long param_5)

{
  long lVar1;
  long lVar2;
  
  if (param_4 < param_2) {
    KOBOOLOXSPAIPP(0x25,"factora.c",0xae);
  }
  if ((param_2 < 1) || (param_4 < 1)) {
    lVar2 = 1;
  }
  else {
    lVar1 = 1;
    lVar2 = 1;
    do {
      if (*(long *)(param_1 + lVar2 * 8) == *(long *)(param_3 + lVar1 * 8)) {
        *(long *)(param_5 + lVar2 * 8) = lVar1;
        lVar2 = lVar2 + 1;
      }
    } while ((lVar2 <= param_2) && (lVar1 = lVar1 + 1, lVar1 <= param_4));
  }
  if (lVar2 <= param_2) {
    KOBOOLOXSPAIPP(0x25,"factora.c",0xbc);
    return;
  }
  return;
}


