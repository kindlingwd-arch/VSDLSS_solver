/* driver: MOFKQcIBJXSPAIPP addr=00978ef0 size=28 */
#include "vsdlss_ref.h"


void MOFKQcIBJXSPAIPP(long param_1,long param_2)

{
  printf("%ld\t%ld\t%ld\t%f\n",*(undefined8 *)(*(long *)(param_1 + 0x40) + param_2 * 8),param_2,
         *(undefined8 *)(*(long *)(param_1 + 0x28) + param_2 * 8),
         *(undefined8 *)(*(long *)(param_1 + 0x38) + param_2 * 8));
  return;
}


