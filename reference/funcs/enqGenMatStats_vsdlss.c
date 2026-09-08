/* VSDLSS function (decompiled by Ghidra) */
/* name: enqGenMatStats_vsdlss  addr: 009e77b0  size: 5b */
#include "vsdlss_ref.h"

/* signature: uint enqGenMatStats_vsdlss(void); */

uint enqGenMatStats_vsdlss(void)

{
  FILE *__stream;
  uint local_c;
  
  local_c = 0;
  __stream = fopen("genmatstats.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,&DAT_00ab841c,&local_c);
    if (4 < local_c) {
      local_c = 0;
    }
    fclose(__stream);
  }
  return local_c;
}


