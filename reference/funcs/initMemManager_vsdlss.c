/* VSDLSS function (decompiled by Ghidra) */
/* name: initMemManager_vsdlss  addr: 009fbac0  size: e0 */
#include "vsdlss_ref.h"

/* signature: long initMemManager_vsdlss(long param_1,int param_2); */

long initMemManager_vsdlss(long param_1,int param_2)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined4 *puVar4;
  
  iVar1 = enqSolveInMem_vsdlss();
  if ((iVar1 == 0) || ((param_2 != 4 && (param_2 != 2)))) {
    if (DAT_00e3d068 != 0) {
      DAT_00e3d068 = 0;
      param_1 = param_1 * DAT_00ec4168;
      puVar2 = &DAT_00ec4180;
      puVar4 = &DAT_00ec4980;
      DAT_00ec4178 = param_1;
      do {
        *puVar2 = 0;
        *puVar4 = 0xffffffff;
        puVar3 = puVar2 + 1;
        puVar4[0x100] = 0xffffffff;
        puVar2[0x200] = 0;
        puVar4[0x400] = 0;
        puVar2 = puVar3;
        puVar4 = puVar4 + 1;
      } while (puVar3 != (undefined8 *)&DAT_00ec4980);
      return param_1;
    }
    deleteAllTempTags_vsdlss();
    deleteJobFromMemoryJrt_vsdlss();
    iVar1 = getNumJobsInJrt_vsdlss();
    if (iVar1 == 0) {
      DAT_00ec4178 = param_1 * DAT_00ec4168;
      return param_1 * DAT_00ec4168;
    }
  }
  return DAT_00ec4178;
}


