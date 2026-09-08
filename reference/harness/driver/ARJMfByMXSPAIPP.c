/* driver: ARJMfByMXSPAIPP addr=009b2b00 size=7f */
#include "vsdlss_ref.h"


void ARJMfByMXSPAIPP(long param_1)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  
  lVar3 = 1;
  printf("Number of elements in Heap is %ld\n",*(undefined8 *)(param_1 + 8));
  puts("-------------------------------------");
  puts("\tEntry\tCost\tPosition");
  puts("\t-----\t----\t--------");
  if (0 < *(long *)(param_1 + 8)) {
    do {
      plVar2 = (long *)(lVar3 * 0x10 + *(long *)(param_1 + 0x18));
      lVar3 = lVar3 + 1;
      lVar1 = *plVar2;
      printf("\t%ld\t%ld\t%ld\n",lVar1,plVar2[1],
             *(undefined8 *)(*(long *)(param_1 + 0x10) + lVar1 * 8));
    } while (lVar3 <= *(long *)(param_1 + 8));
  }
  return;
}


