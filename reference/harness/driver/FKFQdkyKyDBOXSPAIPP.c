/* driver: FKFQdkyKyDBOXSPAIPP addr=00986cc0 size=dd */
#include "vsdlss_ref.h"


void FKFQdkyKyDBOXSPAIPP(char *param_1,long param_2)

{
  size_t sVar1;
  undefined *puVar2;
  undefined8 *puVar3;
  long lVar4;
  char *__s;
  long *plVar5;
  
  sVar1 = strlen(param_1);
  if (0xfc < (long)sVar1) {
    KOBOOLOXSPAIPP(0x22,"fmanager.c",0x45,0xfc);
  }
  plVar5 = &DAT_00e3ab78;
  __s = &DAT_00e750c1;
  lVar4 = 1;
  do {
    if ((param_2 == 0) || (1 < *plVar5 - 1U)) {
      sprintf(__s,"%s.%s",param_1,*(undefined8 *)(&UNK_00b1bde0 + lVar4 * 8));
    }
    else {
      sprintf(__s,"%s.%s",param_2,*(undefined8 *)(&UNK_00b1bde0 + lVar4 * 8));
    }
    lVar4 = lVar4 + 1;
    __s = __s + 0x101;
    plVar5 = plVar5 + 5;
  } while (lVar4 != 0x40);
  puVar3 = &DAT_00e3ab68;
  puVar2 = &DAT_00e750c1;
  do {
    *puVar3 = puVar2;
    puVar2 = puVar2 + 0x101;
    puVar3 = puVar3 + 5;
  } while (puVar2 != &DAT_00e79000);
  return;
}


