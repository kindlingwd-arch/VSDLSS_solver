/* VSDLSS function (decompiled by Ghidra) */
/* name: setPfAndFcInfoPtrs_vsdlss  addr: 009f6700  size: c1 */
#include "vsdlss_ref.h"

/* signature: void setPfAndFcInfoPtrs_vsdlss(void); */

void setPfAndFcInfoPtrs_vsdlss(void)

{
  int iVar1;
  long lVar2;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 == -1) {
    FUN_009f60f0();
  }
  lVar2 = (long)iVar1 * 0x488;
  setPfInfoPtrs_vsdlss
            (*(undefined4 *)(&DAT_00eb1ecc + lVar2),*(undefined8 *)(&DAT_00eb1ed0 + lVar2),
             *(undefined8 *)(&DAT_00eb1ed8 + lVar2),*(undefined8 *)(&DAT_00eb1ee0 + lVar2));
  setFcInfoPtrs_vsdlss
            (*(undefined4 *)(&DAT_00eb1ee8 + lVar2),*(undefined8 *)(&DAT_00eb1ef0 + lVar2),
             *(undefined8 *)(&DAT_00eb1ef8 + lVar2),*(undefined4 *)(&DAT_00eb1f00 + lVar2));
  setPppiInfoPtrs(*(undefined4 *)(&DAT_00eb1f04 + lVar2),*(undefined8 *)(&DAT_00eb1f08 + lVar2),
                  *(undefined8 *)(&DAT_00eb1f10 + lVar2),*(undefined8 *)(&DAT_00eb1f18 + lVar2),
                  *(undefined8 *)(&DAT_00eb1f20 + lVar2),*(undefined8 *)(&DAT_00eb1f28 + lVar2),
                  *(undefined8 *)(&DAT_00eb1f30 + lVar2),*(undefined4 *)(&DAT_00eb1f38 + lVar2),
                  *(undefined8 *)(&DAT_00eb1f40 + lVar2));
  return;
}


