/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDPushComponents_vsdlss  addr: 009fd5f0  size: 1a4 */
#include "vsdlss_ref.h"

/* signature: void MLDPushComponents_vsdlss(int param_1,long param_2,long param_3,long param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7); */

void MLDPushComponents_vsdlss
               (int param_1,long param_2,long param_3,long param_4,undefined8 param_5,
               undefined8 param_6,undefined8 param_7)

{
  long lVar1;
  undefined4 *puVar2;
  int iVar3;
  ulong uVar4;
  int iVar5;
  long lVar6;
  long lVar7;
  undefined4 local_48;
  undefined8 local_40;
  
  if (param_1 < 1) {
    local_40 = 0;
    local_48 = 1;
  }
  else {
    iVar5 = *(int *)(param_3 + 4);
    if ((long)param_1 == 1) {
      local_40 = 4;
      local_48 = 2;
    }
    else {
      uVar4 = 2;
      local_40 = 1;
      do {
        iVar3 = *(int *)(param_3 + uVar4 * 4);
        if (iVar5 < iVar3) {
          local_40 = (long)(int)uVar4;
          iVar5 = iVar3;
        }
        uVar4 = uVar4 + 1;
      } while (uVar4 <= (ulong)(long)param_1);
      local_48 = (int)local_40;
      if (1 < local_48) {
        lVar6 = 0;
        do {
          pushAStack_vsdlss(param_5,*(undefined4 *)(param_2 + 4 + lVar6));
          pushAStack_vsdlss(param_6,*(undefined4 *)(param_3 + 4 + lVar6));
          puVar2 = (undefined4 *)(param_4 + 4 + lVar6);
          lVar6 = lVar6 + 4;
          pushAStack_vsdlss(param_7,*puVar2);
        } while (lVar6 != (ulong)(local_48 - 2) * 4 + 4);
      }
      local_48 = local_48 + 1;
      local_40 = local_40 * 4;
    }
  }
  if (local_48 <= param_1) {
    lVar6 = 0;
    lVar7 = (long)local_48 * 4;
    do {
      pushAStack_vsdlss(param_5,*(undefined4 *)(param_2 + lVar7 + lVar6 * 4));
      pushAStack_vsdlss(param_6,*(undefined4 *)(param_3 + lVar7 + lVar6 * 4));
      lVar1 = lVar6 * 4;
      lVar6 = lVar6 + 1;
      pushAStack_vsdlss(param_7,*(undefined4 *)(lVar7 + param_4 + lVar1));
    } while (local_48 + (int)lVar6 <= param_1);
  }
  pushAStack_vsdlss(param_5,*(undefined4 *)(param_2 + local_40));
  pushAStack_vsdlss(param_6,*(undefined4 *)(param_3 + local_40));
  pushAStack_vsdlss(param_7,*(undefined4 *)(param_4 + local_40));
  return;
}


