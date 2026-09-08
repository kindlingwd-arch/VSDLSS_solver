/* driver: PVJzLIF_cIFJFKyQFLKXSPAIPP addr=009ad270 size=22d */
#include "vsdlss_ref.h"


undefined8
PVJzLIF_cIFJFKyQFLKXSPAIPP
          (undefined8 *param_1,long param_2,long param_3,long param_4,undefined8 param_5,
          long param_6)

{
  long lVar1;
  long lVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  undefined8 local_70;
  long *local_40 [2];
  
  local_40[0] = (long *)*param_1;
  lVar2 = *local_40[0];
  uVar4 = KBTYCCB_QBAZVaERKHXSPAIPP(*(undefined8 *)(param_3 + 0x10));
  lVar5 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  ABzRDnOFKQXSPAIPP("Start symbolicElimination\n");
  MBOJRQBgtB_QLOgKnIy_BXSPAIPP(param_5,lVar2,param_4);
  MBOJRQBeOyMEXSPAIPP(local_40,param_5);
  _LKSBOQe2ejrXSPAIPP(local_40[0]);
  _LKSBOQejr2esrXSPAIPP(local_40[0]);
  lVar8 = 1;
  local_70 = LMBKdy_QLOaERKHdFIBuXSPAIPP(param_3,1);
  ABzRDnOFKQXSPAIPP("symbolicElimination initialization complete\n");
  if (0 < lVar2) {
    lVar7 = 1;
    lVar10 = 1;
    lVar9 = lVar8;
    do {
      lVar1 = lVar10 * 8;
      lVar8 = lVar9;
      if (*(long *)(param_6 + lVar10 * 8) != lVar9) {
        lVar8 = lVar9 + 1;
        _ILPBdy_QLOaERKHdFIBXSPAIPP(param_3,lVar9);
        local_70 = LMBKdy_QLOaERKHdFIBuXSPAIPP(param_3,lVar8);
      }
      if (*(long *)(*(long *)(param_2 + 0x10) + lVar7 * 8) == lVar10) {
        uVar6 = CLOJYKAuOFQBZIL_HgKAtB_
                          (lVar10,lVar7,*(undefined8 *)(param_4 + lVar10 * 8),
                           *(undefined8 *)(local_40[0][4] + lVar1),local_70,lVar5);
        *(undefined8 *)(*(long *)(param_2 + 0x20) + lVar7 * 8) = uVar6;
      }
      puVar3 = *(undefined8 **)(local_40[0][4] + lVar1);
      PBQYCCB_QBAZVaERKH(lVar10,puVar3,param_6,uVar4);
      if (puVar3 != (undefined8 *)0x0) {
        _EB_HZIL_HgKQBDOFQVYKAkBODB(lVar10,*puVar3,lVar7,param_2,local_40[0]);
      }
      lVar10 = lVar10 + 1;
      lVar9 = local_40[0][4];
      uVar6 = ABIBQBcKQFOBacADBIFPQXSPAIPP(*(undefined8 *)(lVar1 + lVar9));
      *(undefined8 *)(lVar1 + lVar9) = uVar6;
      lVar7 = lVar7 + (ulong)(*(long *)(*(long *)(param_2 + 0x10) + lVar7 * 8) +
                              *(long *)(*(long *)(param_2 + 0x18) + lVar7 * 8) <= lVar10);
      lVar9 = lVar8;
    } while (lVar10 <= lVar2);
  }
  _ILPBdy_QLOaERKHdFIBXSPAIPP(param_3,lVar8);
  if (lVar5 != 0) {
    _yPFdOBBXSPAIPP();
  }
  ABzRDnOFKQXSPAIPP("End symbolicElimination\n");
  *param_1 = local_40[0];
  return uVar4;
}


