/* driver: MOFKQgKQYOOyVXSPAIPP addr=00960060 size=3f */
#include "vsdlss_ref.h"


void MOFKQgKQYOOyVXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  
  lVar2 = 1;
  if (0 < param_2) {
    do {
      lVar1 = lVar2 * 8;
      lVar2 = lVar2 + 1;
      printf("%ld\t",*(undefined8 *)(param_1 + lVar1));
    } while (lVar2 <= param_2);
  }
  putchar(10);
  return;
}


