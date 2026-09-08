/* driver: DBQeOyMEdOLJcIBJYOOXSPAIPP addr=009799f0 size=ed */
#include "vsdlss_ref.h"


long DBQeOyMEdOLJcIBJYOOXSPAIPP(undefined8 param_1,long param_2,long param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  long local_38;
  long local_30;
  
  uVar6 = *(undefined8 *)(param_3 + 0x10);
  uVar2 = *(undefined8 *)(param_3 + 0x18);
  lVar3 = KBTeOyMEXSPAIPP();
  uVar4 = KBTacADBtB_QLOXSPAIPP(param_1,1,0);
  *(undefined8 *)(lVar3 + 0x20) = uVar4;
  lVar7 = 0;
  FKFQpByAdOLJrLXSPAIPP(uVar6,uVar2,param_2);
  lVar5 = DBQlBUQdOLJrLXSPAIPP(&local_38,&local_30);
  while (lVar5 == 0) {
    lVar7 = lVar7 + 2;
    puVar1 = (undefined8 *)(*(long *)(lVar3 + 0x20) + local_38 * 8);
    uVar6 = KBTacADBIFPQXSPAIPP(local_30,*puVar1);
    *puVar1 = uVar6;
    puVar1 = (undefined8 *)(*(long *)(lVar3 + 0x20) + local_30 * 8);
    uVar6 = KBTacADBIFPQXSPAIPP(local_38,*puVar1);
    *puVar1 = uVar6;
    lVar5 = DBQlBUQdOLJrLXSPAIPP(&local_38,&local_30);
  }
  CFKFPEpByAdOLJrLXSPAIPP();
  if (param_2 * 2 != lVar7) {
    KOBOOLOXSPAIPP(1,"elemarrutils.c",0x26c);
  }
  *(long *)(lVar3 + 8) = lVar7;
  BIFJFKyQBbRMIF_yQBcADBPgKeOyMEXSPAIPP(lVar3);
  return lVar3;
}


