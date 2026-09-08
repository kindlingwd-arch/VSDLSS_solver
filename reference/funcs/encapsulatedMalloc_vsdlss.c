/* VSDLSS function (decompiled by Ghidra) */
/* name: encapsulatedMalloc_vsdlss  addr: 009e7660  size: 29 */
#include "vsdlss_ref.h"

/* signature: undefined8 encapsulatedMalloc_vsdlss(undefined8 param_1); */

undefined8 encapsulatedMalloc_vsdlss(undefined8 param_1)

{
  undefined8 uVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_00ead8e0);
  uVar1 = (*appMallocFunPtr_vsdlss)(param_1);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_00ead8e0);
  return uVar1;
}


