/* VSDLSS function (decompiled by Ghidra) */
/* name: setFcInfoJrt_vsdlss  addr: 009f6270  size: 54 */
#include "vsdlss_ref.h"

/* signature: void setFcInfoJrt_vsdlss(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4); */

void setFcInfoJrt_vsdlss(undefined4 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4
                        )

{
  int iVar1;
  long lVar2;
  
  iVar1 = findJobInJrtReturn();
  if (iVar1 == -1) {
    FUN_009f60f0();
  }
  lVar2 = (long)iVar1 * 0x488;
  *(undefined4 *)(&DAT_00eb1f00 + lVar2) = param_4;
  *(undefined8 *)(&DAT_00eb1ef8 + lVar2) = param_3;
  *(undefined8 *)(&DAT_00eb1ef0 + lVar2) = param_2;
  *(undefined4 *)(&DAT_00eb1ee8 + lVar2) = param_1;
  return;
}


