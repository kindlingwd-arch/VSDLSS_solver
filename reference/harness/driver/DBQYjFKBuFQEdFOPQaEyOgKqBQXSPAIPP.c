/* driver: DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP addr=0095aa20 size=65 */
#include "vsdlss_ref.h"


long DBQYjFKBuFQEdFOPQaEyOgKqBQXSPAIPP(undefined8 param_1,char *param_2,char *param_3)

{
  bool bVar1;
  long lVar2;
  char *pcVar3;
  char local_28 [16];
  
  do {
    lVar2 = DBQYjFKBXSPAIPP(param_3,0x3ff,param_1);
    bVar1 = false;
    if (lVar2 != 0) {
      return lVar2;
    }
    do {
      if (bVar1) {
        return 0;
      }
      local_28[0] = *param_3;
      local_28[1] = 0;
      pcVar3 = strpbrk(local_28,param_2);
      bVar1 = true;
    } while (pcVar3 != (char *)0x0);
  } while( true );
}


