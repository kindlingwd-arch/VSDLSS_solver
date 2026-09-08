/* VSDLSS function (decompiled by Ghidra) */
/* name: checkAndAllocBuffOnTag_vsdlss  addr: 009fb6a0  size: 123 */
#include "vsdlss_ref.h"

/* signature: void checkAndAllocBuffOnTag_vsdlss(int param_1,ulong param_2); */

void checkAndAllocBuffOnTag_vsdlss(int param_1,ulong param_2)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  
  lVar5 = (long)param_1;
  lVar3 = (&DAT_00ec4180)[lVar5];
  if (lVar3 == 0) {
    nrerror_vsdlss(7,"memmanager.c",0x10a,param_1,param_1);
    lVar3 = (&DAT_00ec4180)[lVar5];
  }
  if (*(ulong *)(lVar3 + (long)(int)(&DAT_00ec4d80)[lVar5] * 0x10) < param_2) {
    if (0xfffe < (int)(&DAT_00ec4980)[lVar5]) {
      nrerror_vsdlss(8,"memmanager.c",0x111,(&DAT_00ec4980)[lVar5],param_1);
    }
    lVar3 = DAT_00ec4178;
    lVar2 = encapsulatedMalloc_vsdlss(DAT_00ec4178 + DAT_00ec4160);
    if (lVar2 == 0) {
      nrerror_vsdlss(9,"memmanager.c",0x11b,DAT_00ec4160 + lVar3,lVar3,DAT_00ec4160,
                     "encapsulatedMalloc");
    }
    iVar1 = *(int *)((long)&DAT_00ec4180 + (lVar5 + 0x200) * 4) + 1;
    *(int *)((long)&DAT_00ec4180 + (lVar5 + 0x200) * 4) = iVar1;
    (&DAT_00ec4d80)[lVar5] = iVar1;
    lVar3 = DAT_00ec4178;
    plVar4 = (long *)((long)iVar1 * 0x10 + (&DAT_00ec4180)[lVar5]);
    plVar4[1] = lVar2;
    *plVar4 = lVar3;
  }
  return;
}


