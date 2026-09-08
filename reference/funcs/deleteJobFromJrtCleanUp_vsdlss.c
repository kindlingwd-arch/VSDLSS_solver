/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteJobFromJrtCleanUp_vsdlss  addr: 009f65a0  size: 35 */
#include "vsdlss_ref.h"

/* signature: void deleteJobFromJrtCleanUp_vsdlss(void); */

void deleteJobFromJrtCleanUp_vsdlss(void)

{
  int iVar1;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 != -1) {
    deleteJobFromMemoryJrt_vsdlss();
    removeSemiPermFiles_vsdlss();
    *(undefined4 *)(&DAT_00eb1ec8 + (long)iVar1 * 0x488) = 0;
    *(undefined4 *)(&DAT_00eb1ac0 + (long)iVar1 * 0x488) = 0;
  }
  return;
}


