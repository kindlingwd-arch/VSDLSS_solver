/* VSDLSS function (decompiled by Ghidra) */
/* name: setPfInfoJrt_vsdlss  addr: 009f6210  size: 56 */
#include "vsdlss_ref.h"

/* signature: void setPfInfoJrt_vsdlss(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4); */

void setPfInfoJrt_vsdlss(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4
                        )

{
  int iVar1;
  long lVar2;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 == -1) {
    FUN_009f60f0();
  }
  lVar2 = (long)iVar1 * 0x488;
  *(undefined8 *)(&DAT_00eb1ee0 + lVar2) = param_4;
  *(undefined8 *)(&DAT_00eb1ed8 + lVar2) = param_3;
  *(undefined8 *)(&DAT_00eb1ed0 + lVar2) = param_2;
  *(undefined4 *)(&DAT_00eb1ecc + lVar2) = param_1;
  return;
}


