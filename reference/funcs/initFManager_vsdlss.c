/* VSDLSS function (decompiled by Ghidra) */
/* name: initFManager_vsdlss  addr: 009f34f0  size: d5 */
#include "vsdlss_ref.h"

/* signature: void initFManager_vsdlss(char *param_1,long param_2); */

void initFManager_vsdlss(char *param_1,long param_2)

{
  size_t sVar1;
  undefined *puVar2;
  undefined8 *puVar3;
  long lVar4;
  char *__s;
  
  sVar1 = strlen(param_1);
  if (0xfc < (int)sVar1) {
    nrerror_vsdlss(0x22,"fmanager.c",0x45,0xfc);
  }
  __s = &DAT_00eada61;
  lVar4 = 0;
  do {
    if ((param_2 == 0) || (1 < (&DAT_00e3c88c)[lVar4] - 1)) {
      sprintf(__s,"%s.%s",param_1,*(undefined8 *)((long)&PTR_DAT_00b23548 + lVar4));
    }
    else {
      sprintf(__s,"%s.%s",param_2,*(undefined8 *)((long)&PTR_DAT_00b23548 + lVar4));
    }
    lVar4 = lVar4 + 8;
    __s = __s + 0x101;
  } while (lVar4 != 0x1f8);
  puVar3 = &DAT_00e3c880;
  puVar2 = &DAT_00eada61;
  do {
    *puVar3 = puVar2;
    puVar2 = puVar2 + 0x101;
    puVar3 = puVar3 + 4;
  } while (puVar2 != &DAT_00eb19a0);
  return;
}


