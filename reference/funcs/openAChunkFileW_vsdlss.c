/* VSDLSS function (decompiled by Ghidra) */
/* name: openAChunkFileW_vsdlss  addr: 009e2950  size: 8d */
#include "vsdlss_ref.h"

/* signature: FILE * openAChunkFileW_vsdlss(long param_1,int param_2); */

FILE * openAChunkFileW_vsdlss(long param_1,int param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  getAChunkFullFn_vsdlss(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x30) + (long)param_2 * 8) != 0) {
    nrerror_vsdlss(0x14,"chunkinfoutils.c",0x16c,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"wb");
  if (pFVar1 == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"chunkinfoutils.c",0x173,acStack_418,"write binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x30) + (long)param_2 * 8) = pFVar1;
  return pFVar1;
}


