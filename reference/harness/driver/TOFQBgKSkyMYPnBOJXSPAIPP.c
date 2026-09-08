/* driver: TOFQBgKSkyMYPnBOJXSPAIPP addr=0096e110 size=4a */
#include "vsdlss_ref.h"


void TOFQBgKSkyMYPnBOJXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  
  lVar1 = KBTgtB_QLOXSPAIPP(param_1,0,0);
  DBQgKSBOPBkyMXSPAIPP(param_2,param_1,lVar1);
  TOFQBgtB_QLOdlRJXSPAIPP(0x26,param_1,lVar1);
  if (lVar1 != 0) {
    _yPFdOBBXSPAIPP(lVar1);
    return;
  }
  return;
}


