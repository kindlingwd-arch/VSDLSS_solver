/* VSDLSS function (decompiled by Ghidra) */
/* name: getNumJobsInJrt_vsdlss  addr: 009f6150  size: 28 */
#include "vsdlss_ref.h"

/* signature: int getNumJobsInJrt_vsdlss(void); */

int getNumJobsInJrt_vsdlss(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = &DAT_00eb1f48;
  iVar1 = 0;
  do {
    iVar1 = (iVar1 + 1) - (uint)(*piVar2 == 0);
    piVar2 = piVar2 + 0x122;
  } while (piVar2 != (int *)&DAT_00ec4148);
  return iVar1;
}


