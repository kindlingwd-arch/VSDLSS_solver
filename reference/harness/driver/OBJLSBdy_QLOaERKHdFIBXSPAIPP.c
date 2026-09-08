/* driver: OBJLSBdy_QLOaERKHdFIBXSPAIPP addr=00974bf0 size=44 */
#include "vsdlss_ref.h"


void OBJLSBdy_QLOaERKHdFIBXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  char acStack_418 [1032];
  
  _ILPBdy_QLOaERKHdFIBXSPAIPP();
  lVar1 = Cy_QLOaERKHdFIBcUFPQPXSPAIPP(param_1,param_2);
  if (lVar1 != 0) {
    DBQdy_QLOaERKHdRIIdKXSPAIPP(param_1,param_2,acStack_418);
    remove(acStack_418);
  }
  return;
}


