/* VSDLSS function (decompiled by Ghidra) */
/* name: saveJobToDiskFreeMemoryJrt_vsdlss  addr: 009f65e0  size: 106 */
#include "vsdlss_ref.h"

/* signature: void saveJobToDiskFreeMemoryJrt_vsdlss(void); */

void saveJobToDiskFreeMemoryJrt_vsdlss(void)

{
  int *piVar1;
  int iVar2;
  long lVar3;
  
  iVar2 = findJobInJrtReturn();
  if (iVar2 == -1) {
    return;
  }
  lVar3 = (long)iVar2 * 0x488;
  if (*(int *)(&DAT_00eb1ec8 + lVar3) == 0) {
    if (*(int *)(&DAT_00eb1ec4 + lVar3) == 0) {
      nrerror_vsdlss(1,"jobreloadmanager.c",0x123);
      deleteJobFromMemoryJrt_vsdlss();
      return;
    }
    nrerror_vsdlss(0x45,"jobreloadmanager.c",0x10e);
    if (-1 < *(int *)(&DAT_00eb1ecc + lVar3)) {
      write_DEG123REORDERVEC_vsdlss
                (*(undefined8 *)(&DAT_00eb1ed0 + lVar3),**(undefined4 **)(&DAT_00eb1ed8 + lVar3));
      writeDeg1Deg2Pf_vsdlss(*(undefined8 *)(&DAT_00eb1ed8 + lVar3));
      writeDeg3Pf_vsdlss(*(undefined8 *)(&DAT_00eb1ee0 + lVar3));
      piVar1 = *(int **)(&DAT_00eb1ed8 + lVar3);
      if (0 < *piVar1 - (piVar1[1] + piVar1[2] + *(int *)(*(long *)(&DAT_00eb1ee0 + lVar3) + 4))) {
        writeFactorOneChunk_vsdlss(*(undefined8 *)(&DAT_00eb1ef0 + lVar3));
        writeReorderVec_vsdlss
                  (*(undefined8 *)(&DAT_00eb1ef8 + lVar3),
                   *(undefined4 *)(*(long *)(&DAT_00eb1ef0 + lVar3) + 8));
      }
    }
    *(undefined4 *)(&DAT_00eb1ec8 + (long)iVar2 * 0x488) = 1;
  }
  deleteJobFromMemoryJrt_vsdlss();
  return;
}


