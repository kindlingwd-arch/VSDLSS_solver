/* VSDLSS function (decompiled by Ghidra) */
/* name: forwardSubstitute_vsdlss  addr: 00a2e290  size: 131 */
#include "vsdlss_ref.h"

/* signature: void forwardSubstitute_vsdlss(long param_1,long param_2,undefined8 param_3); */

void forwardSubstitute_vsdlss(long param_1,long param_2,undefined8 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  long lVar6;
  int iVar7;
  int iVar8;
  char *local_50;
  int local_40;
  
  iVar1 = *(int *)(param_1 + 8);
  uVar4 = getMaxIVector_vsdlss(*(undefined8 *)(param_2 + 0x10),*(undefined4 *)(param_2 + 4));
  uVar5 = newFactorBlockHeader_vsdlss(uVar4);
  local_40 = 1;
  if (0 < iVar1) {
    do {
      local_50 = "\x01";
      do {
        iVar8 = 0;
        casiTimes_vsdlss(t_work1_vsdlss);
        lVar6 = readFactorChunk_vsdlss(local_40,*(undefined4 *)local_50,param_1,param_2);
        casiTimes_vsdlss(t_work2_vsdlss);
        accumDiffTimes_vsdlss(t_work1_vsdlss,t_work2_vsdlss,t_lowerSolveIO_vsdlss);
        iVar2 = *(int *)(lVar6 + 0x14);
        iVar3 = *(int *)(lVar6 + 0x18);
        if (0 < iVar2) {
          do {
            setFactorBlockPtrs_vsdlss(iVar3 + iVar8,param_2,lVar6,uVar5);
            solveWithBlockLower(uVar5,param_3);
            iVar7 = iVar8 + 2;
            iVar8 = iVar8 + 1;
          } while (iVar7 <= iVar2);
        }
        deleteFactorChunk_vsdlss(lVar6);
        local_50 = local_50 + 4;
      } while (local_50 != "\n  Begin Sparse Matrix %s with %d columns\n");
      local_40 = local_40 + 1;
    } while (local_40 <= iVar1);
  }
  deleteFactorBlock_vsdlss(uVar5);
  return;
}


