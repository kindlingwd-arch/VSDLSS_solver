/* VSDLSS function (decompiled by Ghidra) */
/* name: encapsulatedFree_vsdlss  addr: 009e7690  size: 22 */
#include "vsdlss_ref.h"

/* signature: void encapsulatedFree_vsdlss(undefined8 param_1); */

void encapsulatedFree_vsdlss(undefined8 param_1)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00ead8e0);
  (*appFreeFunPtr_vsdlss)(param_1);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00ead8e0);
  return;
}


