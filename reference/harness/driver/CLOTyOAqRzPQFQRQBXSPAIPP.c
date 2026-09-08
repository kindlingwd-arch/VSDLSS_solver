/* driver: CLOTyOAqRzPQFQRQBXSPAIPP addr=009ae3d0 size=141 */
#include "vsdlss_ref.h"


void CLOTyOAqRzPQFQRQBXSPAIPP(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long local_58;
  long local_50;
  
  lVar1 = *(long *)(param_1 + 0x10);
  uVar3 = DBQkyUgtB_QLOXSPAIPP(*(undefined8 *)(param_2 + 0x18),*(undefined8 *)(param_2 + 8));
  uVar3 = KBTdy_QLOZIL_HfByABOXSPAIPP(uVar3);
  local_50 = 1;
  if (0 < lVar1) {
    do {
      local_58 = 1;
      do {
        lVar6 = 1;
        _yPFrFJBPXSPAIPP(QXTLOH1XSPAIPP);
        lVar4 = OByAdy_QLOaERKHXSPAIPP
                          (local_50,*(undefined8 *)(&UNK_00b1dde0 + local_58 * 8),param_1,param_2);
        _yPFrFJBPXSPAIPP(QXTLOH2XSPAIPP);
        y__RJbFCCrFJBPXSPAIPP(QXTLOH1XSPAIPP,QXTLOH2XSPAIPP,QXILTBOqLISBgmXSPAIPP);
        lVar2 = *(long *)(lVar4 + 0x28);
        lVar5 = *(long *)(lVar4 + 0x30);
        if (0 < lVar2) {
          do {
            lVar6 = lVar6 + 1;
            PBQdy_QLOZIL_HnQOPXSPAIPP(lVar5,param_2,lVar4,uVar3);
            PLISBuFQEZIL_HjLTBO(uVar3,param_3);
            lVar5 = lVar5 + 1;
          } while (lVar6 <= lVar2);
        }
        ABIBQBdy_QLOaERKHXSPAIPP(lVar4);
        local_58 = local_58 + 1;
      } while (local_58 != 3);
      local_50 = local_50 + 1;
    } while (local_50 <= lVar1);
  }
  ABIBQBdy_QLOZIL_HXSPAIPP(uVar3);
  return;
}


