/* driver: MOFKQcyXSPAIPP addr=00978f50 size=61 */
#include "vsdlss_ref.h"


void MOFKQcyXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar1 = *param_1;
  puts("Element Array Info:\n");
  printf("Num Elems: %ld;\n",lVar1);
  putchar(10);
  puts("Num\tfromNode\ttoNode\tOffDiag");
  lVar2 = 1;
  if (0 < lVar1) {
    do {
      lVar3 = lVar2 + 1;
      MOFKQcIBJXSPAIPP(param_1,lVar2);
      lVar2 = lVar3;
    } while (lVar3 <= lVar1);
  }
  return;
}


