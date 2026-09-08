/* VSDLSS function (decompiled by Ghidra) */
/* name: createTag_vsdlss  addr: 009fb800  size: ed */
#include "vsdlss_ref.h"

/* signature: int createTag_vsdlss(void); */

int createTag_vsdlss(void)

{
  long lVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  
  lVar1 = 0;
  do {
    iVar3 = (int)lVar1;
    if ((&DAT_00ec4180)[lVar1] == 0) goto LAB_009fb82e;
    lVar1 = lVar1 + 1;
  } while (lVar1 != 0x100);
  iVar3 = 0x100;
  nrerror_vsdlss(10,"memmanager.c",0x177);
LAB_009fb82e:
  lVar4 = (long)iVar3;
  lVar1 = encapsulatedMalloc_vsdlss(0x100000);
  (&DAT_00ec4180)[lVar4] = lVar1;
  if (lVar1 == 0) {
    nrerror_vsdlss(4,"memmanager.c",0x17f,"encapsulatedMalloc");
    lVar1 = (&DAT_00ec4180)[lVar4];
  }
  lVar2 = 0;
  do {
    *(undefined8 *)(lVar1 + 8 + lVar2) = 0;
    *(undefined8 *)(lVar1 + lVar2) = 0xffffffffffffffff;
    lVar2 = lVar2 + 0x10;
  } while (lVar2 != 0x100000);
  (&DAT_00ec4d80)[lVar4] = 0xffffffff;
  (&DAT_00ec5980)[lVar4] = 0;
  *(undefined4 *)((long)&DAT_00ec4180 + (lVar4 + 0x200) * 4) = 0xffffffff;
  (&DAT_00ec4180)[lVar4 + 0x200] = 0;
  return iVar3;
}


