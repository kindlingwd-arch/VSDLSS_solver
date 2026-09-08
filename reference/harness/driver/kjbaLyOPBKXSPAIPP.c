/* driver: kjbaLyOPBKXSPAIPP addr=00999070 size=bc */
#include "vsdlss_ref.h"


long kjbaLyOPBKXSPAIPP(long param_1,undefined8 param_2,undefined8 param_3,long param_4,
                      undefined8 param_5)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar4 = 1;
  do {
    if (param_1 <= lVar4) {
      KOBOOLOXSPAIPP(1,"mldorder.c",0x7f7);
    }
    lVar1 = *(long *)(param_4 + lVar4 * 8);
    lVar5 = lVar4 + 1;
    kjbaLyOPBKmKBjBSBIXSPAIPP(param_2,param_3,lVar4,param_4,param_5);
    lVar2 = *(long *)(param_4 + lVar5 * 8);
    lVar3 = *(long *)(lVar2 + 0x10);
    if (lVar3 <= DAT_00e8e610) {
      return lVar5;
    }
    if ((double)*(long *)(lVar1 + 0x10) * DAT_00e8e608 <= (double)lVar3) {
      return lVar5;
    }
    lVar4 = lVar5;
  } while (lVar3 / 2 < *(long *)(lVar2 + 0x18) * 2);
  return lVar5;
}


