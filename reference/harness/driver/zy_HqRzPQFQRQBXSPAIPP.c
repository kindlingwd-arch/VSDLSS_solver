/* driver: zy_HqRzPQFQRQBXSPAIPP addr=009af430 size=124 */
#include "vsdlss_ref.h"


void zy_HqRzPQFQRQBXSPAIPP(long param_1,long param_2,undefined8 param_3)

{
  undefined8 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 *local_50;
  long local_48;
  
  local_48 = *(long *)(param_1 + 0x10);
  uVar1 = DBQkyUgtB_QLOXSPAIPP(*(undefined8 *)(param_2 + 0x18),*(undefined8 *)(param_2 + 8));
  uVar1 = KBTdy_QLOZIL_HfByABOXSPAIPP(uVar1);
  if (0 < local_48) {
    do {
      local_50 = &DAT_00b1ddd0;
      do {
        _yPFrFJBPXSPAIPP(QXTLOH1XSPAIPP);
        lVar2 = OByAdy_QLOaERKHXSPAIPP(local_48,*local_50,param_1,param_2);
        _yPFrFJBPXSPAIPP(QXTLOH2XSPAIPP);
        y__RJbFCCrFJBPXSPAIPP(QXTLOH1XSPAIPP,QXTLOH2XSPAIPP,QXRMMBOqLISBgmXSPAIPP);
        lVar4 = *(long *)(lVar2 + 0x38);
        lVar3 = lVar4 - *(long *)(lVar2 + 0x28);
        if (0 < *(long *)(lVar2 + 0x28)) {
          do {
            lVar5 = lVar4 + -1;
            PBQdy_QLOZIL_HnQOPXSPAIPP(lVar4,param_2,lVar2,uVar1);
            PLISBuFQEZIL_HsMMBO(uVar1,param_3);
            lVar4 = lVar5;
          } while (lVar5 != lVar3);
        }
        ABIBQBdy_QLOaERKHXSPAIPP(lVar2);
        local_50 = local_50 + -1;
      } while (local_50 != (undefined8 *)&DAT_00b1ddc0);
      local_48 = local_48 + -1;
    } while (local_48 != 0);
  }
  ABIBQBdy_QLOZIL_HXSPAIPP(uVar1);
  return;
}


