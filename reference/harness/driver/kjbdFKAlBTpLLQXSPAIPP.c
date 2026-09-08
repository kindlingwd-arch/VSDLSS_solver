/* driver: kjbdFKAlBTpLLQXSPAIPP addr=00999130 size=5a */
#include "vsdlss_ref.h"


long kjbdFKAlBTpLLQXSPAIPP(long param_1,long param_2,long param_3)

{
  char cVar1;
  long lVar2;
  
  lVar2 = param_1 + 1;
  if (lVar2 <= param_2) {
    cVar1 = *(char *)(param_3 + 1 + param_1);
    while (cVar1 != '\0') {
      lVar2 = lVar2 + 1;
      if (param_2 < lVar2) goto LAB_00999157;
      cVar1 = *(char *)(param_3 + lVar2);
    }
    if (lVar2 != -1) {
      return lVar2;
    }
  }
LAB_00999157:
  lVar2 = -1;
  if (1 < param_1) {
    lVar2 = 1;
    cVar1 = *(char *)(param_3 + 1);
    while (cVar1 != '\0') {
      lVar2 = lVar2 + 1;
      if (lVar2 == param_1) {
        return -1;
      }
      cVar1 = *(char *)(param_3 + lVar2);
    }
  }
  return lVar2;
}


