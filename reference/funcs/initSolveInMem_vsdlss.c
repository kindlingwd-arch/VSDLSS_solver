/* VSDLSS function (decompiled by Ghidra) */
/* name: initSolveInMem_vsdlss  addr: 009e7ad0  size: 5d */
#include "vsdlss_ref.h"

/* signature: void initSolveInMem_vsdlss(uint param_1); */

void initSolveInMem_vsdlss(uint param_1)

{
  FILE *__stream;
  uint local_c;
  
  local_c = param_1;
  __stream = fopen("solveinmem.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,&DAT_00ab841c,&local_c);
    if (2 < local_c) {
      local_c = 0;
    }
    fclose(__stream);
  }
  DAT_00ead914 = local_c;
  return;
}


