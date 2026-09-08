/* VSDLSS function (decompiled by Ghidra) */
/* name: openAChunkFileR_vsdlss  addr: 009e29f0  size: 8d */
#include "vsdlss_ref.h"

/* signature: FILE * openAChunkFileR_vsdlss(long param_1,int param_2); */

FILE * openAChunkFileR_vsdlss(long param_1,int param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  getAChunkFullFn_vsdlss(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x30) + (long)param_2 * 8) != 0) {
    nrerror_vsdlss(0x14,"chunkinfoutils.c",400,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"rb");
  if (pFVar1 == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"chunkinfoutils.c",0x197,acStack_418,"read binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x30) + (long)param_2 * 8) = pFVar1;
  return pFVar1;
}


