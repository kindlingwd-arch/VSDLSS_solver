/* driver: _EB_HYKAYIIL_ZRCCmKryDXSPAIPP addr=0098ff60 size=12d */
#include "vsdlss_ref.h"


void _EB_HYKAYIIL_ZRCCmKryDXSPAIPP(long param_1,ulong param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  
  lVar4 = (&DAT_00e8be00)[param_1];
  if (lVar4 == 0) {
    KOBOOLOXSPAIPP(7,"memmanager.c",0x10a,param_1);
    lVar4 = (&DAT_00e8be00)[param_1];
  }
  if (*(ulong *)(lVar4 + (&DAT_00e8be00)[param_1 + 0x200] * 0x10) < param_2) {
    lVar4 = param_1 + 0x100;
    if (0xfffe < (long)(&DAT_00e8be00)[lVar4]) {
      KOBOOLOXSPAIPP(8,"memmanager.c",0x111,(&DAT_00e8be00)[lVar4],param_1);
    }
    lVar1 = DAT_00e8bdf8;
    lVar3 = BK_yMPRIyQBAkyIIL_XSPAIPP(DAT_00e8bdf8 + DAT_00e8bde0);
    if (lVar3 == 0) {
      KOBOOLOXSPAIPP(9,"memmanager.c",0x11b,DAT_00e8bde0 + lVar1,lVar1,DAT_00e8bde0,
                     "encapsulatedMalloc");
    }
    lVar2 = DAT_00e8bdf8;
    lVar1 = (&DAT_00e8be00)[lVar4] + 1;
    (&DAT_00e8be00)[lVar4] = lVar1;
    (&DAT_00e8be00)[param_1 + 0x200] = lVar1;
    plVar5 = (long *)(lVar1 * 0x10 + (&DAT_00e8be00)[param_1]);
    plVar5[1] = lVar3;
    *plVar5 = lVar2;
  }
  return;
}


