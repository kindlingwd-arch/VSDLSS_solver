/* driver: _yPFpyAFUqLOQXSPAIPP addr=00962080 size=219 */
#include "vsdlss_ref.h"


void _yPFpyAFUqLOQXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long local_160 [12];
  long local_100;
  long alStack_f8 [11];
  long alStack_a0 [14];
  
  local_160[0] = *(long *)(param_3 + 8);
  if (1 < param_1) {
    plVar1 = (long *)(param_3 + 0x10);
    do {
      lVar6 = *plVar1;
      if (*plVar1 <= local_160[0]) {
        lVar6 = local_160[0];
      }
      plVar1 = plVar1 + 1;
      local_160[0] = lVar6;
    } while (plVar1 != (long *)(param_3 + 8 + param_1 * 8));
  }
  lVar6 = 1;
  plVar1 = (long *)(param_3 + 8 + param_1 * 8);
  do {
    plVar2 = local_160 + 2;
    do {
      *plVar2 = 0;
      plVar2 = plVar2 + 1;
    } while (plVar2 != &local_100);
    plVar2 = (long *)(param_3 + 8);
    if (0 < param_1) {
      do {
        lVar4 = *plVar2;
        plVar2 = plVar2 + 1;
        local_160[(lVar4 / lVar6) % 10 + 2] = local_160[(lVar4 / lVar6) % 10 + 2] + 1;
      } while (plVar2 != plVar1);
    }
    alStack_a0[2] = 1;
    alStack_f8[1] = 1;
    lVar4 = 1;
    lVar3 = 2;
    while( true ) {
      lVar5 = local_160[lVar3];
      alStack_a0[lVar3 + 1] = lVar4 + lVar5;
      alStack_f8[lVar3] = lVar4 + lVar5;
      lVar3 = lVar3 + 1;
      if (lVar3 == 0xb) break;
      lVar4 = alStack_a0[lVar3];
    }
    if (0 < param_1) {
      plVar2 = (long *)(param_3 + 8);
      do {
        lVar4 = *plVar2;
        plVar2 = plVar2 + 1;
        lVar5 = (lVar4 / lVar6) % 10 + 1;
        lVar3 = alStack_f8[lVar5];
        *(long *)(param_2 + lVar3 * 8) = lVar4;
        alStack_f8[lVar5] = lVar3 + 1;
      } while (plVar1 != plVar2);
    }
    lVar4 = 1;
    do {
      if (alStack_f8[lVar4] - alStack_a0[lVar4 + 1] != local_160[lVar4 + 1]) {
        KOBOOLOXSPAIPP(1,"utils.c",0x7a2);
      }
      lVar4 = lVar4 + 1;
    } while (lVar4 != 0xb);
    lVar4 = 1;
    if (0 < param_1) {
      do {
        *(undefined8 *)(param_3 + lVar4 * 8) = *(undefined8 *)(param_2 + lVar4 * 8);
        lVar4 = lVar4 + 1;
      } while (lVar4 != param_1 + 1);
    }
    lVar6 = lVar6 * 10;
  } while (lVar6 <= local_160[0]);
  return;
}


