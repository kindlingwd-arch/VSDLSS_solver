/* driver: OBJLSBYIIaERKHdFIBPXSPAIPP addr=00974ce0 size=44 */
#include "vsdlss_ref.h"


void OBJLSBYIIaERKHdFIBPXSPAIPP(char *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  if (((param_1 != (char *)0x0) && (lVar1 = *(long *)(param_1 + 0x10), *param_1 != '\0')) &&
     (0 < lVar1)) {
    lVar2 = 1;
    do {
      OBJLSBdy_QLOaERKHdFIBXSPAIPP(param_1,lVar2);
      lVar3 = lVar2 + 1;
      OBJLSBYaERKHdFIBXSPAIPP(param_1,lVar2);
      lVar2 = lVar3;
    } while (lVar3 <= lVar1);
  }
  return;
}


