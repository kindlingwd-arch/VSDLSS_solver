/* driver: BK_yMPRIyQBAkyIIL_XSPAIPP addr=00979e10 size=29 */
#include "vsdlss_ref.h"


undefined8 BK_yMPRIyQBAkyIIL_XSPAIPP(undefined8 param_1)

{
  undefined8 uVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00e74f20);
  uVar1 = (*yMMkyIIL_dRKnQOXSPAIPP)(param_1);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00e74f20);
  return uVar1;
}


