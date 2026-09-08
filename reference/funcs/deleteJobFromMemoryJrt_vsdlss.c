/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteJobFromMemoryJrt_vsdlss  addr: 009f6510  size: 86 */
#include "vsdlss_ref.h"

/* signature: void deleteJobFromMemoryJrt_vsdlss(void); */

void deleteJobFromMemoryJrt_vsdlss(void)

{
  int iVar1;
  long lVar2;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 != -1) {
    lVar2 = (long)iVar1;
    if (*(int *)(&DAT_00eb1ec4 + lVar2 * 0x488) != 0) {
      if (-1 < *(int *)(&DAT_00eb1ecc + lVar2 * 0x488)) {
        deleteTag_vsdlss();
      }
      if (-1 < *(int *)(&DAT_00eb1ee8 + lVar2 * 0x488)) {
        deleteTag_vsdlss();
      }
      if (-1 < *(int *)(&DAT_00eb1f04 + lVar2 * 0x488)) {
        deleteTag_vsdlss();
      }
      *(undefined4 *)(&DAT_00eb1ec4 + lVar2 * 0x488) = 0;
      initPfAndFcEntriesByPosJrt(iVar1);
      return;
    }
  }
  return;
}


