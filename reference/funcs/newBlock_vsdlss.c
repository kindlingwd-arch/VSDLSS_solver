/* VSDLSS function (decompiled by Ghidra) */
/* name: newBlock_vsdlss  addr: 009fb460  size: 213 */
#include "vsdlss_ref.h"

/* signature: long newBlock_vsdlss(int param_1,ulong param_2); */

long newBlock_vsdlss(int param_1,ulong param_2)

{
  ulong uVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  int iVar4;
  long lVar5;
  long *plVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  ulong *puVar10;
  double dVar11;
  
  uVar9 = DAT_00ec4168;
  if (param_2 % DAT_00ec4168 == 0) {
LAB_009fb49c:
    lVar7 = (long)param_1 + 0x200;
    if (-1 < (long)param_2) goto LAB_009fb4af;
LAB_009fb5c3:
    dVar11 = (double)param_2;
  }
  else {
    uVar9 = DAT_00ec4170;
    if (param_2 % DAT_00ec4170 == 0) goto LAB_009fb49c;
    lVar7 = (long)param_1 + 0x200;
    param_2 = (param_2 / DAT_00ec4170 + 1) * DAT_00ec4170;
    if ((long)param_2 < 0) goto LAB_009fb5c3;
LAB_009fb4af:
    dVar11 = (double)(long)param_2;
  }
  lVar8 = (long)param_1;
  lVar5 = (&DAT_00ec4180)[lVar8];
  (&DAT_00ec4180)[lVar7] = dVar11 + (double)(&DAT_00ec4180)[lVar7];
  if (lVar5 == 0) {
    nrerror_vsdlss(7,"memmanager.c",0xc0,param_1);
  }
  if ((param_2 <= DAT_00ec4178) && (-1 < (int)(&DAT_00ec4d80)[lVar8])) {
    puVar10 = (ulong *)((long)(int)(&DAT_00ec4d80)[lVar8] * 0x10 + (&DAT_00ec4180)[lVar8]);
    uVar1 = *puVar10;
    auVar2._8_8_ = 0;
    auVar2._0_8_ = uVar9;
    auVar3._8_8_ = 0;
    auVar3._0_8_ = uVar1;
    lVar7 = SUB168(auVar3 % auVar2,0);
    if (param_2 <= uVar1 - (long)SUB164(auVar3 % auVar2,0)) {
      lVar5 = (DAT_00ec4178 - uVar1) + lVar7 + puVar10[1];
      *puVar10 = (uVar1 - param_2) - lVar7;
      goto LAB_009fb58f;
    }
  }
  if (0xfffe < (int)(&DAT_00ec4980)[lVar8]) {
    nrerror_vsdlss(8,"memmanager.c",0xd9,(&DAT_00ec4980)[lVar8],param_1);
  }
  uVar9 = DAT_00ec4178;
  if (DAT_00ec4178 < param_2) {
    uVar9 = param_2;
  }
  lVar5 = encapsulatedMalloc_vsdlss(uVar9 + DAT_00ec4160);
  if (lVar5 == 0) {
    nrerror_vsdlss(9,"memmanager.c",0xee,uVar9 + DAT_00ec4160,uVar9,DAT_00ec4160,
                   "encapsulatedMalloc");
  }
  iVar4 = *(int *)((long)&DAT_00ec4180 + (lVar8 + 0x200) * 4) + 1;
  *(int *)((long)&DAT_00ec4180 + (lVar8 + 0x200) * 4) = iVar4;
  uVar1 = DAT_00ec4178;
  if (DAT_00ec4178 == uVar9) {
    (&DAT_00ec4d80)[lVar8] = iVar4;
  }
  plVar6 = (long *)((long)iVar4 * 0x10 + (&DAT_00ec4180)[lVar8]);
  plVar6[1] = lVar5;
  *plVar6 = uVar1 - param_2;
LAB_009fb58f:
  return lVar5 + DAT_00ec4160;
}


