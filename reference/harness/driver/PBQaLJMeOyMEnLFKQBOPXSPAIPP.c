/* driver: PBQaLJMeOyMEnLFKQBOPXSPAIPP addr=00997a30 size=18e */
#include "vsdlss_ref.h"


long PBQaLJMeOyMEnLFKQBOPXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,undefined8 *param_6
               ,long *param_7)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  
  lVar2 = param_3 * 8 + -8;
  param_6[1] = 0;
  uVar3 = *(undefined8 *)(param_1 + 8);
  param_6[10] = 0;
  param_6[0x11] = 0;
  *param_6 = 0xffffffffffffffff;
  param_6[3] = 0;
  param_6[0xb] = 0;
  param_6[1] = uVar3;
  lVar6 = *(long *)(param_1 + 0x88);
  lVar7 = lVar2 + *(long *)(param_1 + 0x50);
  param_6[0xc] = 0;
  param_6[0xd] = 0;
  param_6[0xe] = 0;
  param_6[0xf] = 0;
  param_6[0x10] = 0;
  param_6[2] = param_4;
  param_6[0x11] = lVar2 + lVar6;
  param_6[10] = lVar7;
  lVar6 = param_4;
  if (*(long *)(param_1 + 0x80) != 0) {
    lVar8 = *(long *)(param_1 + 0x80) + lVar2;
    param_6[0x10] = lVar8;
    if (param_4 < 1) {
      lVar6 = 0;
    }
    else {
      lVar5 = 1;
      lVar6 = 0;
      do {
        while (lVar8 != 0) {
          lVar1 = lVar5 * 8;
          lVar5 = lVar5 + 1;
          lVar6 = lVar6 + *(long *)(lVar8 + lVar1);
          if (lVar5 == param_4 + 1) goto LAB_00997b30;
        }
        lVar5 = lVar5 + 1;
        lVar6 = lVar6 + 1;
      } while (lVar5 != param_4 + 1);
    }
  }
LAB_00997b30:
  param_6[0xc] = lVar2 + *(long *)(param_1 + 0x60);
  lVar8 = param_5 * 8 + -8;
  param_6[0xd] = lVar8 + *(long *)(param_1 + 0x68);
  if (*(long *)(param_1 + 0x70) != 0) {
    lVar5 = *(long *)(param_1 + 0x78);
    param_6[0xe] = *(long *)(param_1 + 0x70) + lVar2;
    param_6[0xf] = lVar8 + lVar5;
  }
  uVar4 = PRJgtB_QLOcIBJPXSPAIPP(lVar7,param_4);
  if ((uVar4 & 1) != 0) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0x48c);
  }
  param_6[3] = (long)uVar4 / 2;
  *param_7 = lVar6;
  return param_2 + lVar2;
}


