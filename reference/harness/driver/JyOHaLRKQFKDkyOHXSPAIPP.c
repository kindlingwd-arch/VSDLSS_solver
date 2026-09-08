/* driver: JyOHaLRKQFKDkyOHXSPAIPP addr=00975610 size=49 */
#include "vsdlss_ref.h"


void JyOHaLRKQFKDkyOHXSPAIPP(long *param_1,long param_2)

{
  long *plVar1;
  
  if ((param_2 < 1) || (param_1[1] < param_2)) {
    KOBOOLOXSPAIPP(1,"countingmarkutils.c",0x8b);
  }
  plVar1 = (long *)(param_1[2] + param_2 * 8);
  if (*plVar1 < *param_1) {
    *plVar1 = *param_1;
  }
  return;
}


