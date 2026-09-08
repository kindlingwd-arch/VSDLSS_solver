/* driver: PySBhLzrLbFPHdOBBkBJLOVhOQXSPAIPP addr=0098b180 size=11b */
#include "vsdlss_ref.h"


void PySBhLzrLbFPHdOBBkBJLOVhOQXSPAIPP(void)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  
  lVar2 = CFKAhLzgKhOQpBQROK();
  if (lVar2 == -1) {
    return;
  }
  lVar3 = lVar2 * 0x4a0;
  if (*(long *)(&DAT_00e79530 + lVar3) == 0) {
    if (*(long *)(&DAT_00e79528 + lVar3) == 0) {
      KOBOOLOXSPAIPP(1,"jobreloadmanager.c",0x123);
      ABIBQBhLzdOLJkBJLOVhOQXSPAIPP();
      return;
    }
    KOBOOLOXSPAIPP(0x45,"jobreloadmanager.c",0x10e);
    if (-1 < *(long *)(&DAT_00e79538 + lVar3)) {
      TOFQBXbce123pcmpbcptcaXSPAIPP
                (*(undefined8 *)(&DAT_00e79540 + lVar3),**(undefined8 **)(&DAT_00e79548 + lVar3));
      TOFQBbBD1bBD2nCXSPAIPP(*(undefined8 *)(&DAT_00e79548 + lVar3));
      TOFQBbBD3nCXSPAIPP(*(undefined8 *)(&DAT_00e79550 + lVar3));
      plVar1 = *(long **)(&DAT_00e79548 + lVar3);
      if (0 < *plVar1 - (plVar1[1] + plVar1[2] + *(long *)(*(long *)(&DAT_00e79550 + lVar3) + 8))) {
        TOFQBdy_QLOmKBaERKHXSPAIPP(*(undefined8 *)(&DAT_00e79560 + lVar3));
        TOFQBpBLOABOtB_XSPAIPP
                  (*(undefined8 *)(&DAT_00e79568 + lVar3),
                   *(undefined8 *)(*(long *)(&DAT_00e79560 + lVar3) + 0x10));
      }
    }
    *(undefined8 *)(&DAT_00e79530 + lVar2 * 0x4a0) = 1;
  }
  ABIBQBhLzdOLJkBJLOVhOQXSPAIPP();
  return;
}


