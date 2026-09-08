/* VSDLSS function (decompiled by Ghidra) */
/* name: openFactorChunkFileW_vsdlss  addr: 009e27d0  size: 8d */
#include "vsdlss_ref.h"

/* signature: FILE * openFactorChunkFileW_vsdlss(long param_1,int param_2); */

FILE * openFactorChunkFileW_vsdlss(long param_1,int param_2)

{
  FILE *pFVar1;
  char acStack_418 [1024];
  
  getFactorChunkFullFn_vsdlss(param_1,param_2,acStack_418);
  if (*(long *)(*(long *)(param_1 + 0x28) + (long)param_2 * 8) != 0) {
    nrerror_vsdlss(0x14,"chunkinfoutils.c",0xff,acStack_418);
  }
  pFVar1 = fopen(acStack_418,"wb");
  if (pFVar1 == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"chunkinfoutils.c",0x106,acStack_418,"write binary");
  }
  *(FILE **)(*(long *)(param_1 + 0x28) + (long)param_2 * 8) = pFVar1;
  return pFVar1;
}


