/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteTag_vsdlss  addr: 009fb930  size: 9a */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteTag_vsdlss(int param_1); */

undefined8 deleteTag_vsdlss(int param_1)

{
  undefined8 *puVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  
  lVar4 = (long)param_1;
  lVar3 = 0;
  iVar2 = 0;
  if (-1 < (int)(&DAT_00ec4980)[lVar4]) {
    do {
      iVar2 = iVar2 + 1;
      puVar1 = (undefined8 *)((&DAT_00ec4180)[lVar4] + 8 + lVar3);
      lVar3 = lVar3 + 0x10;
      encapsulatedFree_vsdlss(*puVar1);
    } while (iVar2 <= (int)(&DAT_00ec4980)[lVar4]);
  }
  encapsulatedFree_vsdlss((&DAT_00ec4180)[lVar4]);
  (&DAT_00ec4180)[lVar4] = 0;
  (&DAT_00ec4d80)[lVar4] = 0xffffffff;
  *(undefined4 *)((long)&DAT_00ec4180 + (lVar4 + 0x200) * 4) = 0xffffffff;
  (&DAT_00ec4180)[lVar4 + 0x200] = 0;
  (&DAT_00ec5980)[lVar4] = 0;
  return 0;
}


