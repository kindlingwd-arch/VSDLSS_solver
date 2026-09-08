/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDMinCoverAugment_vsdlss  addr: 00a011f0  size: 11b */
#include "vsdlss_ref.h"

/* signature: void MLDMinCoverAugment_vsdlss(int param_1,long param_2,long param_3,long param_4); */

void MLDMinCoverAugment_vsdlss(int param_1,long param_2,long param_3,long param_4)

{
  char *pcVar1;
  uint *puVar2;
  char cVar3;
  int iVar4;
  long lVar5;
  uint uVar6;
  ulong uVar7;
  uint *puVar8;
  
  uVar7 = (ulong)param_1;
  do {
    lVar5 = (long)*(int *)(param_2 + (long)param_1 * 4);
    if (*(char *)(param_3 + lVar5) == '\x02') {
      lVar5 = (long)*(int *)(param_2 + uVar7 * 4);
      pcVar1 = (char *)(param_3 + lVar5);
      cVar3 = *pcVar1;
      while (cVar3 != '\x02') {
        *pcVar1 = '\x02';
        lVar5 = (long)*(int *)(param_2 + (long)*(int *)(param_2 + lVar5 * 4) * 4);
        pcVar1 = (char *)(param_3 + lVar5);
        cVar3 = *pcVar1;
      }
      return;
    }
    param_1 = *(int *)(param_2 + lVar5 * 4);
  } while (param_1 != 0);
  if (*(int *)(param_4 + lVar5 * 4) != 0) {
    nrerror_vsdlss(1,"mldorder.c",0xc70);
  }
  do {
    uVar6 = (uint)uVar7;
    iVar4 = *(int *)(param_2 + (long)(int)uVar6 * 4);
    lVar5 = (long)iVar4 * 4;
    puVar2 = (uint *)(param_4 + lVar5);
    puVar8 = (uint *)(lVar5 + param_2);
    if (*puVar2 != *puVar8) {
      nrerror_vsdlss(1,"mldorder.c",0xc81);
    }
    *(int *)(param_4 + (long)(int)uVar6 * 4) = iVar4;
    *puVar2 = uVar6;
    *(undefined1 *)(param_3 + iVar4) = 2;
    uVar6 = *puVar8;
    uVar7 = (ulong)uVar6;
  } while (uVar6 != 0);
  return;
}


