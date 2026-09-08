/* VSDLSS function (decompiled by Ghidra) */
/* name: copySupNodeValsToLongVec_vsdlss  addr: 009ee0f0  size: 80 */
#include "vsdlss_ref.h"

/* signature: void copySupNodeValsToLongVec_vsdlss(int param_1,int param_2,int param_3,long param_4,long param_5); */

void copySupNodeValsToLongVec_vsdlss(int param_1,int param_2,int param_3,long param_4,long param_5)

{
  int iVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  uint uVar5;
  long lVar6;
  
  uVar5 = param_2 - param_1;
  lVar2 = *(long *)(param_4 + 0x38);
  iVar1 = *(int *)(*(long *)(param_4 + 0x28) + (long)param_1 * 4);
  if (param_3 <= iVar1) {
    lVar6 = (long)param_3 * 8;
    iVar4 = 0;
    do {
      if (-1 < (int)uVar5) {
        lVar3 = 0;
        do {
          *(undefined8 *)(param_5 + (long)iVar4 * 8 + 8 + lVar3) =
               *(undefined8 *)(*(long *)(lVar2 + (long)param_1 * 8 + lVar3) + (lVar6 - lVar3));
          lVar3 = lVar3 + 8;
        } while (lVar3 != (ulong)uVar5 * 8 + 8);
        iVar4 = uVar5 + 1 + iVar4;
      }
      lVar6 = lVar6 + 8;
    } while (lVar6 != ((long)param_3 + 1 + (ulong)(uint)(iVar1 - param_3)) * 8);
  }
  return;
}


