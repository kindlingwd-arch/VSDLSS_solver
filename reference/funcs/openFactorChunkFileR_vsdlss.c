/* VSDLSS function (decompiled by Ghidra) */
/* name: openFactorChunkFileR_vsdlss  addr: 009e2870  size: 8d */
#include "vsdlss_ref.h"

/* signature: FILE * openFactorChunkFileR_vsdlss(long param_1,int param_2); */

FILE * openFactorChunkFileR_vsdlss(long param_1,int param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  getFactorChunkFullFn_vsdlss(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x28) + (long)param_2 * 8) != 0) {
    nrerror_vsdlss(0x14,"chunkinfoutils.c",0x123,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"rb");
  if (pFVar1 == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"chunkinfoutils.c",0x12a,acStack_418,"read binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x28) + (long)param_2 * 8) = pFVar1;
  return pFVar1;
}


