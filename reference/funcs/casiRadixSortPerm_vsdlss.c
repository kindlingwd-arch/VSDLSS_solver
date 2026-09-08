/* VSDLSS function (decompiled by Ghidra) */
/* name: casiRadixSortPerm_vsdlss  addr: 009d9cd0  size: 28c */
#include "vsdlss_ref.h"

/* signature: void casiRadixSortPerm_vsdlss(int param_1,long param_2,long param_3,long param_4,long param_5); */

void casiRadixSortPerm_vsdlss(int param_1,long param_2,long param_3,long param_4,long param_5)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  int iVar8;
  int local_ec;
  int aiStack_c8 [11];
  int local_9c;
  int aiStack_98 [13];
  int local_64 [13];
  
  local_ec = *(int *)(param_3 + 4);
  if (1 < param_1) {
    piVar4 = (int *)(param_3 + 8);
    do {
      if (local_ec < *piVar4) {
        local_ec = *piVar4;
      }
      piVar4 = piVar4 + 1;
    } while (piVar4 != (int *)(param_3 + 0xc + (ulong)(param_1 - 2) * 4));
  }
  iVar6 = 1;
  lVar1 = (ulong)(param_1 - 1) * 4 + 4;
  do {
    piVar4 = aiStack_c8 + 1;
    do {
      *piVar4 = 0;
      piVar4 = piVar4 + 1;
    } while (piVar4 != &local_9c);
    piVar4 = (int *)(param_3 + 4);
    if (0 < param_1) {
      do {
        iVar5 = *piVar4;
        piVar4 = piVar4 + 1;
        aiStack_c8[(iVar5 / iVar6) % 10 + 1] = aiStack_c8[(iVar5 / iVar6) % 10 + 1] + 1;
      } while (piVar4 != (int *)(param_3 + 8) + (param_1 - 1));
    }
    local_64[0] = 1;
    aiStack_98[1] = 1;
    iVar5 = 1;
    lVar7 = 0;
    while( true ) {
      iVar5 = iVar5 + *(int *)((long)aiStack_c8 + lVar7 + 4);
      *(int *)((long)local_64 + lVar7 + 4) = iVar5;
      *(int *)((long)aiStack_98 + lVar7 + 8) = iVar5;
      if (lVar7 + 4 == 0x24) break;
      iVar5 = *(int *)((long)local_64 + lVar7 + 4);
      lVar7 = lVar7 + 4;
    }
    if (0 < param_1) {
      lVar7 = 0;
      do {
        iVar5 = *(int *)(param_3 + 4 + lVar7);
        iVar8 = (iVar5 / iVar6) % 10 + 1;
        iVar2 = aiStack_98[iVar8];
        *(int *)(param_2 + (long)iVar2 * 4) = iVar5;
        uVar3 = *(undefined4 *)(param_4 + 4 + lVar7);
        lVar7 = lVar7 + 4;
        aiStack_98[iVar8] = iVar2 + 1;
        *(undefined4 *)(param_5 + (long)iVar2 * 4) = uVar3;
      } while (lVar7 != lVar1);
    }
    lVar7 = 0;
    do {
      if (*(int *)((long)aiStack_98 + lVar7 + 4) - *(int *)((long)local_64 + lVar7) !=
          *(int *)((long)aiStack_c8 + lVar7 + 4)) {
        nrerror_vsdlss(1,"utils.c",0x819);
      }
      lVar7 = lVar7 + 4;
    } while (lVar7 != 0x28);
    lVar7 = 0;
    if (0 < param_1) {
      do {
        *(undefined4 *)(param_3 + 4 + lVar7) = *(undefined4 *)(param_2 + 4 + lVar7);
        *(undefined4 *)(param_4 + 4 + lVar7) = *(undefined4 *)(param_5 + 4 + lVar7);
        lVar7 = lVar7 + 4;
      } while (lVar1 != lVar7);
    }
    iVar6 = iVar6 * 10;
  } while (iVar6 <= local_ec);
  return;
}


