/* VSDLSS function (decompiled by Ghidra) */
/* name: getBaseJobName_vsdlss  addr: 009d4d30  size: 58 */
#include "vsdlss_ref.h"

/* signature: void getBaseJobName_vsdlss(char *param_1,char *param_2); */

void getBaseJobName_vsdlss(char *param_1,char *param_2)

{
  long lVar1;
  size_t sVar2;
  long lVar3;
  long lVar4;
  
  sVar2 = strlen(param_1);
  if ((int)sVar2 < 0) {
LAB_009d4d73:
    lVar3 = 0;
  }
  else {
    lVar3 = (long)(int)sVar2;
    if (param_1[lVar3] != '/') {
      lVar4 = lVar3 + -1;
      lVar1 = lVar4;
      do {
        lVar3 = lVar1;
        if (lVar3 == lVar4 - (sVar2 & 0xffffffff)) goto LAB_009d4d73;
        lVar1 = lVar3 + -1;
      } while (param_1[lVar3] != '/');
    }
    lVar3 = lVar3 + 1;
  }
  strcpy(param_2,param_1 + lVar3);
  return;
}


