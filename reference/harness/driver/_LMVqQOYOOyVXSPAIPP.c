/* driver: _LMVqQOYOOyVXSPAIPP addr=00961c50 size=4d */
#include "vsdlss_ref.h"


void _LMVqQOYOOyVXSPAIPP(long param_1,ulong param_2,long param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if (param_2 != 0) {
    do {
      lVar2 = uVar3 * 8;
      lVar1 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      strcpy(*(char **)(param_3 + lVar1),*(char **)(param_1 + lVar2));
    } while (uVar3 <= param_2);
  }
  return;
}


