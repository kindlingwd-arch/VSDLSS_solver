/* driver: JyQkyQkRIQX_yPFXSPAIPP addr=0097e2c0 size=95 */
#include "vsdlss_ref.h"


void JyQkyQkRIQX_yPFXSPAIPP
               (undefined8 param_1,long param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5,undefined8 param_6,long param_7)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  if (param_7 != 0) {
    lVar2 = 1;
    if (0 < param_2) {
      do {
        lVar1 = lVar2 + 0xb;
        if (param_2 <= lVar2 + 0xb) {
          lVar1 = param_2;
        }
        lVar3 = lVar2 + 0xc;
        JyQkyQkRIQX_yPFXPO_pyKDBX1(param_1,param_2,param_3,param_4,param_5,param_6,lVar2,lVar1);
        lVar2 = lVar3;
      } while (lVar3 <= param_2);
    }
    return;
  }
  JyQkyQkRIQX_yPFXABPQpyKDB(param_1);
  return;
}


