/* VSDLSS function (decompiled by Ghidra) */
/* name: enqDenseSolve_vsdlss  addr: 009e7760  size: 4a */
#include "vsdlss_ref.h"

/* signature: undefined4 enqDenseSolve_vsdlss(void); */

undefined4 enqDenseSolve_vsdlss(void)

{
  FILE *__stream;
  undefined4 local_c;
  
  local_c = 0;
  __stream = fopen("dsolve.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,&DAT_00ab841c,&local_c);
    fclose(__stream);
  }
  return local_c;
}


