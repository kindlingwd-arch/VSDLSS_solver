/* driver: DBQZyPBhLzlyJBXSPAIPP addr=0095d4c0 size=4e */
#include "vsdlss_ref.h"


void DBQZyPBhLzlyJBXSPAIPP(char *param_1,char *param_2)

{
  char cVar1;
  size_t sVar2;
  size_t sVar3;
  size_t sVar4;
  
  sVar2 = strlen(param_1);
  if ((long)sVar2 < 0) {
LAB_0095d4f8:
    sVar4 = 0;
  }
  else {
    cVar1 = param_1[sVar2];
    sVar4 = sVar2 + 1;
    while (cVar1 != '/') {
      sVar3 = sVar2 - 1;
      if (sVar3 == 0xffffffffffffffff) goto LAB_0095d4f8;
      sVar4 = sVar2;
      sVar2 = sVar3;
      cVar1 = param_1[sVar3];
    }
  }
  strcpy(param_2,param_1 + sVar4);
  return;
}


