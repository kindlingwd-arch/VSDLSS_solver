/* driver: LMBKYaERKHdFIBuXSPAIPP addr=00974a20 size=8a */
#include "vsdlss_ref.h"


FILE * LMBKYaERKHdFIBuXSPAIPP(long param_1,long param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  DBQYaERKHdRIIdKXSPAIPP(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x40) + param_2 * 8) != 0) {
    KOBOOLOXSPAIPP(0x14,"chunkinfoutils.c",0x16c,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"wb");
  if (pFVar1 == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"chunkinfoutils.c",0x173,acStack_418,"write binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x40) + param_2 * 8) = pFVar1;
  return pFVar1;
}


