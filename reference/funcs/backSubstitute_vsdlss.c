/* VSDLSS function (decompiled by Ghidra) */
/* name: backSubstitute_vsdlss  addr: 00a2f350  size: 113 */
#include "vsdlss_ref.h"

/* signature: void backSubstitute_vsdlss(long param_1,long param_2,undefined8 param_3); */

void backSubstitute_vsdlss(long param_1,long param_2,undefined8 param_3)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  undefined4 *local_50;
  int local_44;
  
  local_44 = *(int *)(param_1 + 8);
  uVar3 = getMaxIVector_vsdlss(*(undefined8 *)(param_2 + 0x10),*(undefined4 *)(param_2 + 4));
  uVar4 = newFactorBlockHeader_vsdlss(uVar3);
  if (0 < local_44) {
    do {
      local_50 = &DAT_00b24460;
      do {
        casiTimes_vsdlss(t_work1_vsdlss);
        lVar5 = readFactorChunk_vsdlss(local_44,*local_50,param_1,param_2);
        casiTimes_vsdlss(t_work2_vsdlss);
        accumDiffTimes_vsdlss(t_work1_vsdlss,t_work2_vsdlss,t_upperSolveIO_vsdlss);
        iVar1 = *(int *)(lVar5 + 0x1c);
        iVar2 = *(int *)(lVar5 + 0x14);
        iVar6 = iVar1;
        if (0 < iVar2) {
          do {
            iVar7 = iVar6 + -1;
            setFactorBlockPtrs_vsdlss(iVar6,param_2,lVar5,uVar4);
            solveWithBlockUpper(uVar4,param_3);
            iVar6 = iVar7;
          } while (iVar7 != iVar1 - iVar2);
        }
        deleteFactorChunk_vsdlss(lVar5);
        local_50 = local_50 + -1;
      } while (local_50 != (undefined4 *)&DAT_00b24458);
      local_44 = local_44 + -1;
    } while (local_44 != 0);
  }
  deleteFactorBlock_vsdlss(uVar4);
  return;
}


