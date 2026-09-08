/* driver: kjbZyIyK_BXSPAIPP addr=00999d70 size=7c */
#include "vsdlss_ref.h"


void kjbZyIyK_BXSPAIPP(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  
  lVar1 = *(long *)(param_1 + 8);
  lVar2 = *(long *)(param_2 + 0x38);
  lVar3 = *(long *)(param_2 + 0x40);
  uVar4 = lVar1 - lVar2 >> 0x3f;
  if (((((lVar2 + lVar3) * 3) / *(long *)(param_2 + 0x10) <= (long)((lVar1 - lVar2 ^ uVar4) - uVar4)
       ) && ((lVar2 <= lVar1 || ((long)((double)lVar1 * DAT_00e8e618) <= lVar2)))) &&
     ((lVar3 <= *(long *)(param_1 + 0x10) ||
      ((long)((double)*(long *)(param_1 + 0x10) * DAT_00e8e618) <= lVar3)))) {
    kjbZyIyK_BZLRKAyOVlLABPXE(param_1,param_2,param_3);
    return;
  }
  return;
}


