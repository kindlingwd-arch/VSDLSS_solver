/* driver: DBQkyUqQOjBKXSPAIPP addr=00961bd0 size=76 */
#include "vsdlss_ref.h"


size_t DBQkyUqQOjBKXSPAIPP(long param_1,long param_2)

{
  size_t sVar1;
  size_t sVar2;
  undefined8 *puVar3;
  
  if ((0 < param_2) && (param_1 != 0)) {
    sVar1 = strlen(*(char **)(param_1 + 8));
    if (param_2 != 1) {
      puVar3 = (undefined8 *)(param_1 + 0x10);
      do {
        sVar2 = strlen((char *)*puVar3);
        if ((long)sVar1 < (long)sVar2) {
          sVar1 = sVar2;
        }
        puVar3 = puVar3 + 1;
      } while (puVar3 != (undefined8 *)(param_1 + 8 + param_2 * 8));
    }
    return sVar1;
  }
  return 0;
}


