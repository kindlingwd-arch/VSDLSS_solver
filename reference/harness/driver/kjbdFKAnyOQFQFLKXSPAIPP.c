/* driver: kjbdFKAnyOQFQFLKXSPAIPP addr=0099afc0 size=123 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void kjbdFKAnyOQFQFLKXSPAIPP
               (undefined8 param_1,undefined8 param_2,long param_3,long param_4,undefined8 param_5)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined1 auStack_58 [8];
  long local_50;
  long local_48;
  
  if (*(long *)(param_3 + 0x10) < 1) {
    lVar3 = 0;
    lVar4 = 0;
    lVar1 = 0;
  }
  else {
    lVar3 = *(long *)(param_3 + 0x10) + 1;
    lVar4 = 1;
    lVar1 = 0;
    do {
      while (*(long *)(param_3 + 0x80) != 0) {
        lVar2 = lVar4 * 8;
        lVar4 = lVar4 + 1;
        lVar1 = lVar1 + *(long *)(*(long *)(param_3 + 0x80) + lVar2);
        if (lVar4 == lVar3) goto LAB_0099b030;
      }
      lVar4 = lVar4 + 1;
      lVar1 = lVar1 + 1;
    } while (lVar4 != lVar3);
LAB_0099b030:
    lVar4 = lVar1 / 2;
    lVar3 = lVar1 - lVar4;
  }
  *(undefined8 *)(param_3 + 8) = 1;
  *(long *)(param_4 + 8) = param_3;
  lVar2 = (long)((double)(lVar1 / DAT_00e8e610) * _DAT_00e8e628);
  lVar1 = 2;
  if (1 < lVar2) {
    lVar1 = lVar2;
  }
  lVar2 = kjbaLyOPBKXSPAIPP(param_1,param_2,lVar1,param_4,param_5);
  local_50 = lVar4;
  local_48 = lVar3;
  kjbgKFQnyOQFQFLKXSPAIPP(param_2,auStack_58,*(undefined8 *)(param_4 + lVar2 * 8),param_5);
  lVar1 = lVar2;
  if (1 < lVar2) {
    do {
      lVar3 = lVar1 + -1;
      kjbsK_LyOPBKpBCFKBmKBjBSBIXSPAIPP(auStack_58,lVar1,lVar2,param_4,param_5);
      lVar1 = lVar3;
    } while (lVar3 != 1);
  }
  return;
}


