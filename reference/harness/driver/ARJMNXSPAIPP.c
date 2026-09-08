/* driver: ARJMNXSPAIPP addr=009a7e50 size=e2 */
#include "vsdlss_ref.h"


void ARJMNXSPAIPP(long *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar3 = param_1[1];
  lVar2 = *param_1;
  if (lVar3 == lVar2) {
    puts("Queue is empty.");
    return;
  }
  if (lVar3 < lVar2) {
    if (lVar2 <= param_1[2]) {
      lVar3 = lVar2 * 8;
      do {
        lVar2 = lVar2 + 1;
        puVar1 = (undefined8 *)(param_1[3] + lVar3);
        lVar3 = lVar3 + 8;
        printf(" %ld",*puVar1);
      } while (lVar2 <= param_1[2]);
      lVar3 = param_1[1];
    }
    if (1 < lVar3) {
      lVar3 = 1;
      do {
        lVar2 = lVar3 * 8;
        lVar3 = lVar3 + 1;
        printf(" %ld",*(undefined8 *)(param_1[3] + lVar2));
      } while (lVar3 < param_1[1]);
    }
  }
  else {
    lVar4 = lVar2 * 8;
    if (lVar2 < lVar3) {
      do {
        lVar2 = lVar2 + 1;
        puVar1 = (undefined8 *)(param_1[3] + lVar4);
        lVar4 = lVar4 + 8;
        printf(" %ld",*puVar1);
      } while (lVar2 < param_1[1]);
      putchar(10);
      return;
    }
  }
  putchar(10);
  return;
}


