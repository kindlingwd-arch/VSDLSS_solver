/* driver: KBTZIL_HXSPAIPP addr=0098fd10 size=214 */
#include "vsdlss_ref.h"


long KBTZIL_HXSPAIPP(long param_1,ulong param_2)

{
  long lVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  ulong *puVar6;
  ulong uVar7;
  bool bVar8;
  double dVar9;
  
  uVar7 = DAT_00e8bde8;
  if (param_2 % DAT_00e8bde8 == 0) {
LAB_0098fd4e:
    if (-1 < (long)param_2) goto LAB_0098fd5f;
LAB_0098fe79:
    dVar9 = (double)param_2;
  }
  else {
    uVar7 = DAT_00e8bdf0;
    if (param_2 % DAT_00e8bdf0 == 0) goto LAB_0098fd4e;
    param_2 = (param_2 / DAT_00e8bdf0 + 1) * DAT_00e8bdf0;
    if ((long)param_2 < 0) goto LAB_0098fe79;
LAB_0098fd5f:
    dVar9 = (double)(long)param_2;
  }
  (&DAT_00e8be00)[param_1 + 0x300] = dVar9 + (double)(&DAT_00e8be00)[param_1 + 0x300];
  if ((&DAT_00e8be00)[param_1] == 0) {
    KOBOOLOXSPAIPP(7,"memmanager.c",0xc0,param_1);
  }
  if ((param_2 <= DAT_00e8bdf8) && (-1 < (long)(&DAT_00e8ce00)[param_1])) {
    puVar6 = (ulong *)((&DAT_00e8ce00)[param_1] * 0x10 + (&DAT_00e8be00)[param_1]);
    uVar2 = *puVar6;
    uVar7 = uVar2 % uVar7;
    if (param_2 <= uVar2 - uVar7) {
      lVar3 = (DAT_00e8bdf8 - uVar2) + uVar7 + puVar6[1];
      *puVar6 = (uVar2 - param_2) - uVar7;
      goto LAB_0098fe40;
    }
  }
  lVar1 = param_1 + 0x100;
  if (0xfffe < (long)(&DAT_00e8be00)[lVar1]) {
    KOBOOLOXSPAIPP(8,"memmanager.c",0xd9,(&DAT_00e8be00)[lVar1],param_1);
  }
  uVar7 = DAT_00e8bdf8;
  if (DAT_00e8bdf8 < param_2) {
    uVar7 = param_2;
  }
  lVar3 = BK_yMPRIyQBAkyIIL_XSPAIPP(uVar7 + DAT_00e8bde0);
  if (lVar3 == 0) {
    KOBOOLOXSPAIPP(9,"memmanager.c",0xee,uVar7 + DAT_00e8bde0,uVar7,DAT_00e8bde0,
                   "encapsulatedMalloc");
  }
  uVar2 = DAT_00e8bdf8;
  lVar4 = (&DAT_00e8be00)[lVar1] + 1;
  bVar8 = DAT_00e8bdf8 == uVar7;
  (&DAT_00e8be00)[lVar1] = lVar4;
  if (bVar8) {
    (&DAT_00e8ce00)[param_1] = lVar4;
  }
  plVar5 = (long *)(lVar4 * 0x10 + (&DAT_00e8be00)[param_1]);
  plVar5[1] = lVar3;
  *plVar5 = uVar2 - param_2;
LAB_0098fe40:
  return lVar3 + DAT_00e8bde0;
}


