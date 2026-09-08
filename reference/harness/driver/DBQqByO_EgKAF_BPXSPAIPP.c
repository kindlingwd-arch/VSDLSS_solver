/* driver: DBQqByO_EgKAF_BPXSPAIPP addr=0097b4e0 size=1aa */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DBQqByO_EgKAF_BPXSPAIPP(long param_1,long param_2,void *param_3,size_t param_4,long param_5)

{
  void *pvVar1;
  size_t __nmemb;
  long lVar2;
  long lVar3;
  long local_40 [2];
  
  if ((long)param_4 < param_2) {
    KOBOOLOXSPAIPP(0x25,"factora.c",0x147);
  }
  if ((double)(long)param_4 / (double)param_2 <= _DAT_00b1ab28) {
    if ((param_2 < 1) || ((long)param_4 < 1)) {
      lVar2 = 1;
    }
    else {
      lVar3 = 1;
      lVar2 = 1;
      do {
        if (*(long *)(param_1 + lVar2 * 8) == *(long *)((long)param_3 + lVar3 * 8)) {
          *(long *)(param_5 + lVar2 * 8) = lVar3;
          lVar2 = lVar2 + 1;
        }
      } while ((lVar2 <= param_2) && (lVar3 = lVar3 + 1, lVar3 <= (long)param_4));
    }
    if (lVar2 <= param_2) {
      KOBOOLOXSPAIPP(0x25,"factora.c",0x15b);
      return;
    }
  }
  else if (0 < param_2) {
    lVar3 = 0;
    lVar2 = 1;
    __nmemb = param_4;
    pvVar1 = param_3;
    do {
      while( true ) {
        lVar3 = lVar3 + 1;
        local_40[0] = *(long *)(param_1 + lVar2 * 8);
        if (local_40[0] == *(long *)((long)param_3 + lVar3 * 8)) break;
        pvVar1 = bsearch(local_40,(void *)((long)pvVar1 + 8),__nmemb,8,JVgKQBDBOaLJMyOBXSPAIPP);
        if (pvVar1 == (void *)0x0) {
          KOBOOLOXSPAIPP(0x25,"factora.c",0x177);
        }
        lVar3 = (long)pvVar1 - (long)param_3 >> 3;
        *(long *)(param_5 + lVar2 * 8) = lVar3;
        lVar2 = lVar2 + 1;
        __nmemb = param_4 - lVar3;
        if (param_2 < lVar2) {
          return;
        }
      }
      *(long *)(param_5 + lVar2 * 8) = lVar3;
      lVar2 = lVar2 + 1;
      pvVar1 = (void *)((long)pvVar1 + 8);
      __nmemb = __nmemb - 1;
    } while (lVar2 <= param_2);
  }
  return;
}


