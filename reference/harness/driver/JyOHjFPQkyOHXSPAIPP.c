/* driver: JyOHjFPQkyOHXSPAIPP addr=0098b6c0 size=7a */
#include "vsdlss_ref.h"


void JyOHjFPQkyOHXSPAIPP(long *param_1,long param_2)

{
  char cVar1;
  char *pcVar2;
  long lVar3;
  
  if ((param_2 < 1) || (*param_1 < param_2)) {
    KOBOOLOXSPAIPP(1,"listmarkutils.c",0x70);
    pcVar2 = (char *)(param_2 + param_1[1]);
    cVar1 = *pcVar2;
  }
  else {
    pcVar2 = (char *)(param_2 + param_1[1]);
    cVar1 = *pcVar2;
  }
  if (cVar1 == '\0') {
    *pcVar2 = '\x01';
    lVar3 = KBTacADBIFPQXSPAIPP(param_2,0);
    if (param_1[3] != 0) {
      *(long *)(param_1[3] + 8) = lVar3;
      param_1[3] = lVar3;
      return;
    }
    param_1[3] = lVar3;
    param_1[2] = lVar3;
  }
  return;
}


