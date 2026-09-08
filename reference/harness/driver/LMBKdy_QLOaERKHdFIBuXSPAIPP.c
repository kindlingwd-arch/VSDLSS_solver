/* driver: LMBKdy_QLOaERKHdFIBuXSPAIPP addr=009748a0 size=8a */
#include "vsdlss_ref.h"


FILE * LMBKdy_QLOaERKHdFIBuXSPAIPP(long param_1,long param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  DBQdy_QLOaERKHdRIIdKXSPAIPP(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x38) + param_2 * 8) != 0) {
    KOBOOLOXSPAIPP(0x14,"chunkinfoutils.c",0xff,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"wb");
  if (pFVar1 == (FILE *)0x0) {
    KOBOOLOXSPAIPP(0xe,"chunkinfoutils.c",0x106,acStack_418,"write binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x38) + param_2 * 8) = pFVar1;
  return pFVar1;
}


