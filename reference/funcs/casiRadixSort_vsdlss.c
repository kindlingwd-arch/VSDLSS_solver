/* VSDLSS function (decompiled by Ghidra) */
/* name: casiRadixSort_vsdlss  addr: 009d9840  size: 257 */
#include "vsdlss_ref.h"

/* signature: void casiRadixSort_vsdlss(int param_1,long param_2,long param_3); */

void casiRadixSort_vsdlss(int param_1,long param_2,long param_3)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  int local_ec;
  int aiStack_c8 [11];
  int local_9c;
  int aiStack_98 [13];
  int local_64 [13];
  
  local_ec = *(int *)(param_3 + 4);
  if (1 < param_1) {
    piVar2 = (int *)(param_3 + 8);
    do {
      if (local_ec < *piVar2) {
        local_ec = *piVar2;
      }
      piVar2 = piVar2 + 1;
    } while (piVar2 != (int *)(param_3 + 0xc + (ulong)(param_1 - 2) * 4));
  }
  iVar6 = 1;
  piVar2 = (int *)(param_3 + 8) + (param_1 - 1);
  do {
    piVar3 = aiStack_c8 + 1;
    do {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    } while (piVar3 != &local_9c);
    piVar3 = (int *)(param_3 + 4);
    if (0 < param_1) {
      do {
        iVar4 = *piVar3;
        piVar3 = piVar3 + 1;
        aiStack_c8[(iVar4 / iVar6) % 10 + 1] = aiStack_c8[(iVar4 / iVar6) % 10 + 1] + 1;
      } while (piVar3 != piVar2);
    }
    local_64[0] = 1;
    aiStack_98[1] = 1;
    iVar4 = 1;
    lVar5 = 0;
    while( true ) {
      iVar4 = iVar4 + *(int *)((long)aiStack_c8 + lVar5 + 4);
      *(int *)((long)local_64 + lVar5 + 4) = iVar4;
      *(int *)((long)aiStack_98 + lVar5 + 8) = iVar4;
      if (lVar5 + 4 == 0x24) break;
      iVar4 = *(int *)((long)local_64 + lVar5 + 4);
      lVar5 = lVar5 + 4;
    }
    if (0 < param_1) {
      piVar3 = (int *)(param_3 + 4);
      do {
        iVar4 = *piVar3;
        piVar3 = piVar3 + 1;
        iVar7 = (iVar4 / iVar6) % 10 + 1;
        iVar1 = aiStack_98[iVar7];
        *(int *)(param_2 + (long)iVar1 * 4) = iVar4;
        aiStack_98[iVar7] = iVar1 + 1;
      } while (piVar2 != piVar3);
    }
    lVar5 = 0;
    do {
      if (*(int *)((long)aiStack_98 + lVar5 + 4) - *(int *)((long)local_64 + lVar5) !=
          *(int *)((long)aiStack_c8 + lVar5 + 4)) {
        nrerror_vsdlss(1,"utils.c",0x7a2);
      }
      lVar5 = lVar5 + 4;
    } while (lVar5 != 0x28);
    lVar5 = 0;
    if (0 < param_1) {
      do {
        *(undefined4 *)(param_3 + 4 + lVar5) = *(undefined4 *)(param_2 + 4 + lVar5);
        lVar5 = lVar5 + 4;
      } while (lVar5 != (ulong)(param_1 - 1) * 4 + 4);
    }
    iVar6 = iVar6 * 10;
  } while (iVar6 <= local_ec);
  return;
}


