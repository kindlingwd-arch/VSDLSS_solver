/* driver: OByAkyQOFUdOLJdFIBnOFKQrLjLDXSPAIPP addr=0098a570 size=13e */
#include "vsdlss_ref.h"


void OByAkyQOFUdOLJdFIBnOFKQrLjLDXSPAIPP(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  uVar1 = param_1[1];
  uVar2 = *param_1;
  MOFKQrLjLDXSPAIPP("  Begin Contents of Matrix Files\n");
  MOFKQrLjLDXSPAIPP("  ------------------------------\n");
  MOFKQrLjLDXSPAIPP("\tNumber of Nodes(Variables): %ld\n",uVar1);
  MOFKQrLjLDXSPAIPP("\tNumber of Offdiagonals: %ld\n",uVar2);
  lVar3 = KBTdtB_QLOXSPAIPP(0,uVar1,0);
  OByAdtB_QLOdlRJXSPAIPP(0x1a,uVar1,lVar3);
  MOFKQdtB_QLOrLjLDXSPAIPP("Diagonal Vector",lVar3,uVar1);
  if (lVar3 != 0) {
    _yPFdOBBXSPAIPP(lVar3);
  }
  lVar3 = KBTgtB_QLOXSPAIPP(uVar2,0,0);
  OByAgtB_QLOdlRJXSPAIPP(0x1b,uVar2,lVar3);
  lVar4 = KBTgtB_QLOXSPAIPP(uVar2,0,0);
  OByAgtB_QLOdlRJXSPAIPP(0x1c,uVar2,lVar4);
  lVar5 = KBTdtB_QLOXSPAIPP(0,uVar2,0);
  OByAdtB_QLOdlRJXSPAIPP(0x1d,uVar2,lVar5);
  MOFKQmCCbFyDLKyIPrLjLDXSPAIPP(lVar3,lVar4,lVar5,uVar2);
  if (lVar3 != 0) {
    _yPFdOBBXSPAIPP(lVar3);
  }
  if (lVar4 != 0) {
    _yPFdOBBXSPAIPP(lVar4);
  }
  if (lVar5 != 0) {
    _yPFdOBBXSPAIPP(lVar5);
  }
  MOFKQrLjLDXSPAIPP("  End   Contents of Matrix Files\n");
  MOFKQrLjLDXSPAIPP("  ------------------------------\n");
  return;
}


