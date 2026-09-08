/* driver: DBQYaERKHdRIIdKXSPAIPP addr=00974810 size=8f */
#include "vsdlss_ref.h"


void DBQYaERKHdRIIdKXSPAIPP(long param_1,long param_2,char *param_3)

{
  char *__s;
  size_t sVar1;
  size_t sVar2;
  undefined1 auStack_b8 [152];
  
  sVar1 = strlen(zyPBhLzlyJBXSPAIPP);
  __s = *(char **)(*(long *)(param_1 + 0x28) +
                  *(long *)(*(long *)(param_1 + 0x30) + param_2 * 8) * 8);
  sVar2 = strlen(__s);
  if ((long)(0x3ee - sVar1) < (long)sVar2) {
    KOBOOLOXSPAIPP(0x12,"chunkinfoutils.c",0xdf,sVar2,0x3ee - sVar1);
  }
  DBQYaERKHdKXSPAIPP(param_2,auStack_b8);
  sprintf(param_3,"%s%s",__s,auStack_b8);
  return;
}


