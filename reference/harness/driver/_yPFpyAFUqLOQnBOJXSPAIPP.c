/* driver: _yPFpyAFUqLOQnBOJXSPAIPP addr=00962490 size=258 */
#include "vsdlss_ref.h"


void _yPFpyAFUqLOQnBOJXSPAIPP(long param_1,long param_2,long param_3,long param_4,long param_5)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long local_170;
  long alStack_158 [11];
  long local_100;
  long alStack_f8 [11];
  long alStack_a0 [14];
  
  local_170 = *(long *)(param_3 + 8);
  if (1 < param_1) {
    plVar2 = (long *)(param_3 + 0x10);
    do {
      if (local_170 < *plVar2) {
        local_170 = *plVar2;
      }
      plVar2 = plVar2 + 1;
    } while (plVar2 != (long *)(param_3 + 8 + param_1 * 8));
  }
  lVar5 = 1;
  do {
    plVar2 = alStack_158 + 1;
    do {
      *plVar2 = 0;
      plVar2 = plVar2 + 1;
    } while (plVar2 != &local_100);
    plVar2 = (long *)(param_3 + 8);
    if (0 < param_1) {
      do {
        lVar4 = *plVar2;
        plVar2 = plVar2 + 1;
        alStack_158[(lVar4 / lVar5) % 10 + 1] = alStack_158[(lVar4 / lVar5) % 10 + 1] + 1;
      } while (plVar2 != (long *)(param_3 + 8 + param_1 * 8));
    }
    alStack_a0[2] = 1;
    alStack_f8[1] = 1;
    lVar4 = 1;
    lVar3 = 2;
    while( true ) {
      lVar1 = alStack_158[lVar3 + -1];
      alStack_a0[lVar3 + 1] = lVar4 + lVar1;
      alStack_f8[lVar3] = lVar4 + lVar1;
      lVar3 = lVar3 + 1;
      if (lVar3 == 0xb) break;
      lVar4 = alStack_a0[lVar3];
    }
    if (0 < param_1) {
      lVar4 = 1;
      do {
        lVar3 = *(long *)(param_3 + lVar4 * 8);
        lVar6 = (lVar3 / lVar5) % 10 + 1;
        lVar1 = alStack_f8[lVar6];
        *(long *)(param_2 + lVar1 * 8) = lVar3;
        lVar3 = lVar4 * 8;
        lVar4 = lVar4 + 1;
        *(undefined8 *)(param_5 + lVar1 * 8) = *(undefined8 *)(param_4 + lVar3);
        alStack_f8[lVar6] = lVar1 + 1;
      } while (lVar4 != param_1 + 1);
    }
    lVar4 = 1;
    do {
      if (alStack_f8[lVar4] - alStack_a0[lVar4 + 1] != alStack_158[lVar4]) {
        KOBOOLOXSPAIPP(1,"utils.c",0x819);
      }
      lVar4 = lVar4 + 1;
    } while (lVar4 != 0xb);
    lVar4 = 1;
    if (0 < param_1) {
      do {
        *(undefined8 *)(param_3 + lVar4 * 8) = *(undefined8 *)(param_2 + lVar4 * 8);
        *(undefined8 *)(param_4 + lVar4 * 8) = *(undefined8 *)(param_5 + lVar4 * 8);
        lVar4 = lVar4 + 1;
      } while (lVar4 != param_1 + 1);
    }
    lVar5 = lVar5 * 10;
  } while (lVar5 <= local_170);
  return;
}


