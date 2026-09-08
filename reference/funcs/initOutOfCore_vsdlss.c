/* VSDLSS function (decompiled by Ghidra) */
/* name: initOutOfCore_vsdlss  addr: 009e79c0  size: 5e */
#include "vsdlss_ref.h"

/* signature: void initOutOfCore_vsdlss(int param_1); */

void initOutOfCore_vsdlss(int param_1)

{
  FILE *__stream;
  int local_c;
  
  local_c = param_1;
  __stream = fopen("outofcore.cas","r");
  if (__stream != (FILE *)0x0) {
    __isoc99_fscanf(__stream,&DAT_00ab841c,&local_c);
    if (local_c != 0) {
      local_c = 1;
    }
    fclose(__stream);
  }
  DAT_00ead90c = local_c;
  return;
}


