/* driver: CIRPEayPFkyIIL_dOBBXSPAIPP addr=0096f380 size=4e */
#include "vsdlss_ref.h"


void CIRPEayPFkyIIL_dOBBXSPAIPP(void)

{
  undefined8 *puVar1;
  
  puVar1 = DAT_00e5a528;
  while (puVar1 != (undefined8 *)0x0) {
    DAT_00e5a528 = (undefined8 *)puVar1[1];
    BK_yMPRIyQBAdOBBXSPAIPP(*puVar1);
    puVar1[1] = DAT_00e5a538;
    DAT_00e5a538 = puVar1;
    puVar1 = DAT_00e5a528;
  }
  DAT_00e5a528 = (undefined8 *)0x0;
  return;
}


