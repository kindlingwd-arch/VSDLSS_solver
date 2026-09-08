/* driver: MRPEYqQy_HXSPAIPP addr=009b1e60 size=ce */
#include "vsdlss_ref.h"


void MRPEYqQy_HXSPAIPP(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar3 = param_1[1];
  if (lVar3 == param_1[2]) {
    lVar1 = param_1[3];
    if (lVar3 != lVar1) {
      if (lVar3 * 2 <= lVar1) {
        lVar1 = lVar3 * 2;
      }
      if (*param_1 < 0) {
        lVar2 = KBTgtB_QLOXSPAIPP(lVar1,0,0);
      }
      else {
        lVar2 = QyDlBTgtB_QLOXSPAIPP(*param_1,lVar1,0,0);
      }
      _LMVgtB_QLOXSPAIPP(param_1[4],param_1[2],lVar2);
      if ((*param_1 < 0) && (param_1[4] != 0)) {
        _yPFdOBBXSPAIPP();
      }
      param_1[4] = lVar2;
      param_1[2] = lVar1;
      lVar3 = param_1[1];
      goto LAB_009b1e7e;
    }
    KOBOOLOXSPAIPP(0x35,"astackutils.c",100,"AStack");
    lVar3 = param_1[1];
  }
  lVar2 = param_1[4];
LAB_009b1e7e:
  param_1[1] = lVar3 + 1;
  *(undefined8 *)(lVar2 + (lVar3 + 1) * 8) = param_2;
  return;
}


