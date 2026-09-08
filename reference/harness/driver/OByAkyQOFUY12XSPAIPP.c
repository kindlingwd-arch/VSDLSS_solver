/* driver: OByAkyQOFUY12XSPAIPP addr=009630e0 size=20b */
#include "vsdlss_ref.h"


undefined8 * OByAkyQOFUY12XSPAIPP(long param_1,long param_2)

{
  undefined8 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  long lVar9;
  undefined1 local_c8 [8];
  long local_c0;
  long local_b8;
  
  puVar5 = (undefined8 *)KBTqMyOPBdy_QLOXSPAIPP(param_2,1);
  uVar1 = *puVar5;
  uVar6 = LMBKdFIBpXSPAIPP(0x29);
  OByAgtB_QLOdM(uVar6,1,local_c8);
  lVar3 = local_c0;
  if (local_c0 != param_2) {
    KOBOOLOXSPAIPP(0xc,"vsdlss.c",0x102,param_2,local_c0,&DAT_00b19051);
  }
  lVar9 = 1;
  if (0 < lVar3) {
    do {
      OByAgtB_QLOdM(uVar6,2,local_c8);
      lVar2 = local_b8;
      lVar4 = local_c0;
      if ((param_2 < local_c0) || (local_c0 < 0)) {
        KOBOOLOXSPAIPP(0xf,"vsdlss.c",0x10f,param_2);
        if (lVar2 <= param_1) goto LAB_00963194;
LAB_00963230:
        KOBOOLOXSPAIPP(0x11,"vsdlss.c",0x115,lVar2,param_1);
      }
      else {
        if (param_1 < local_b8) goto LAB_00963230;
LAB_00963194:
        if (lVar2 < 0) goto LAB_00963230;
      }
      uVar7 = 0;
      uVar8 = 0;
      if (lVar2 != 0) {
        uVar7 = QyDlBTgtB_QLOXSPAIPP(uVar1,lVar2,0,0);
        OByAgtB_QLOdM(uVar6,lVar2,uVar7);
        OyKDBaEB_HgtB_QLOXSPAIPP(uVar7,lVar2,1,param_1,"A12 Index Vector");
        uVar8 = QyDlBTbtB_QLOXSPAIPP(0,uVar1,lVar2,0);
        OByAbtB_QLOdM(uVar6,lVar2,uVar8);
      }
      lVar9 = lVar9 + 1;
      *(long *)(puVar5[8] + lVar4 * 8) = lVar2;
      lVar2 = puVar5[0xb];
      uVar8 = QyDlBTgttuFQEnQOPXSPAIPP(uVar1,uVar7,uVar8);
      *(undefined8 *)(lVar2 + lVar4 * 8) = uVar8;
    } while (lVar9 <= lVar3);
  }
  _yPFdaILPBXSPAIPP(0x29);
  return puVar5;
}


