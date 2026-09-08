/* driver: ARJMYqQy_HXSPAIPP addr=009b1f90 size=75 */
#include "vsdlss_ref.h"


void ARJMYqQy_HXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  
  printf("AStack size %ld\n",*(undefined8 *)(param_1 + 0x10));
  if (*(long *)(param_1 + 8) == 0) {
    puts("AStack is empty.");
    return;
  }
  lVar2 = 1;
  puts("AStack is printed base to top.");
  if (0 < *(long *)(param_1 + 8)) {
    do {
      lVar1 = lVar2 * 8;
      lVar2 = lVar2 + 1;
      printf(" %ld\n",*(undefined8 *)(*(long *)(param_1 + 0x20) + lVar1));
    } while (lVar2 <= *(long *)(param_1 + 8));
  }
  return;
}


