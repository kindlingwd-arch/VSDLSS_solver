/* driver: ARJMnNXSPAIPP addr=009a6960 size=7d */
#include "vsdlss_ref.h"


void ARJMnNXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  
  lVar2 = 1;
  printf("Number of elements in Pq is %ld\n",*(undefined8 *)(param_1 + 0x10));
  puts("-------------------------------------");
  puts("\tEntry\tCost\tPosition");
  puts("\t-----\t----\t--------");
  if (0 < *(long *)(param_1 + 0x10)) {
    do {
      lVar1 = *(long *)(*(long *)(param_1 + 0x20) + lVar2 * 8);
      lVar2 = lVar2 + 1;
      printf("\t%ld\t%ld\t%ld\n",lVar1,*(undefined8 *)(*(long *)(param_1 + 0x28) + lVar1 * 8),
             *(undefined8 *)(*(long *)(param_1 + 0x18) + lVar1 * 8));
    } while (lVar2 <= *(long *)(param_1 + 0x10));
  }
  return;
}


