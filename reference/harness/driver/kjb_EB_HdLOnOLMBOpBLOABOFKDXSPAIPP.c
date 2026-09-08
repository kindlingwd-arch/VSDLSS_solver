/* driver: kjb_EB_HdLOnOLMBOpBLOABOFKDXSPAIPP addr=0099cf30 size=109 */
#include "vsdlss_ref.h"


void kjb_EB_HdLOnOLMBOpBLOABOFKDXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  char *pcVar2;
  long lVar3;
  long lVar4;
  
  lVar1 = KBTatB_QLOXSPAIPP(param_1,0,0);
  if (param_2 != param_1) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0xfb6);
  }
  FKFQatB_QLOXSPAIPP(lVar1,param_1,0);
  lVar4 = 1;
  if (0 < param_1) {
LAB_0099cf94:
    do {
      lVar3 = *(long *)(param_3 + lVar4 * 8);
      if (lVar3 < 0) {
        if (DAT_00e8e690 != 0) {
          lVar3 = -lVar3;
          goto LAB_0099cfa1;
        }
        KOBOOLOXSPAIPP(1,"mldorder.c",0xfc3);
LAB_0099d010:
        KOBOOLOXSPAIPP(1,"mldorder.c",0xfc9);
      }
      else {
LAB_0099cfa1:
        if ((param_1 < lVar3) || (lVar3 < 1)) goto LAB_0099d010;
      }
      pcVar2 = (char *)(lVar3 + lVar1);
      if (*pcVar2 == '\0') {
        lVar4 = lVar4 + 1;
        *pcVar2 = '\x01';
        if (param_1 < lVar4) break;
        goto LAB_0099cf94;
      }
      lVar4 = lVar4 + 1;
      KOBOOLOXSPAIPP(1,"mldorder.c",0xfcd);
      *pcVar2 = '\x01';
    } while (lVar4 <= param_1);
  }
  if (lVar1 == 0) {
    return;
  }
  _yPFdOBBXSPAIPP(lVar1);
  return;
}


