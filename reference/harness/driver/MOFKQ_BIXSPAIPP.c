/* driver: MOFKQ_BIXSPAIPP addr=00974220 size=50 */
#include "vsdlss_ref.h"


long MOFKQ_BIXSPAIPP(undefined8 *param_1)

{
  long lVar1;
  
  lVar1 = 0;
  putchar(10);
  for (; param_1 != (undefined8 *)0x0; param_1 = (undefined8 *)param_1[1]) {
    lVar1 = lVar1 + 1;
    printf("%ld ",*param_1);
  }
  putchar(10);
  return lVar1;
}


