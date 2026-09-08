/* driver: LMBKYaERKHdFIBpXSPAIPP addr=00974ac0 size=8a */
#include "vsdlss_ref.h"


FILE * LMBKYaERKHdFIBpXSPAIPP(long param_1,long param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  DBQYaERKHdRIIdKXSPAIPP(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x40) + param_2 * 8) != 0) {
    KOBOOLOXSPAIPP(0x14,"chunkinfoutils.c",400,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"rb");
  if (pFVar1 == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"chunkinfoutils.c",0x197,acStack_418,"read binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x40) + param_2 * 8) = pFVar1;
  return pFVar1;
}


