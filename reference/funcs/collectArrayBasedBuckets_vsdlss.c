/* VSDLSS function (decompiled by Ghidra) */
/* name: collectArrayBasedBuckets_vsdlss  addr: 00a1e670  size: 72 */
#include "vsdlss_ref.h"

/* signature: void collectArrayBasedBuckets_vsdlss(long param_1,long param_2); */

void collectArrayBasedBuckets_vsdlss(long param_1,long param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  
  iVar1 = *(int *)(param_1 + 4);
  lVar3 = *(long *)(param_1 + 0x10);
  lVar4 = *(long *)(param_1 + 8);
  if (0 < iVar1) {
    lVar7 = 0;
    iVar8 = 0;
    do {
      iVar2 = *(int *)(lVar3 + 4 + lVar7);
      lVar5 = *(long *)(lVar4 + 8 + lVar7 * 2);
      if (0 < iVar2) {
        lVar6 = 0;
        do {
          *(undefined4 *)(param_2 + (long)iVar8 * 4 + 4 + lVar6) =
               *(undefined4 *)(lVar5 + 4 + lVar6);
          lVar6 = lVar6 + 4;
        } while (lVar6 != (ulong)(iVar2 - 1) * 4 + 4);
        iVar8 = iVar8 + iVar2;
      }
      lVar7 = lVar7 + 4;
    } while (lVar7 != (ulong)(iVar1 - 1) * 4 + 4);
  }
  return;
}


