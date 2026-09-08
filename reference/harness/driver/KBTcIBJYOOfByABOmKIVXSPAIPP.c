/* driver: KBTcIBJYOOfByABOmKIVXSPAIPP addr=00978d30 size=5c */
#include "vsdlss_ref.h"


void KBTcIBJYOOfByABOmKIVXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x50);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  *(undefined1 *)(puVar1 + 2) = 0;
  puVar1[3] = 0;
  *(undefined1 *)(puVar1 + 4) = 0;
  puVar1[5] = 0;
  puVar1[6] = 0;
  puVar1[7] = 0;
  puVar1[8] = 0;
  puVar1[9] = 0;
  return;
}


