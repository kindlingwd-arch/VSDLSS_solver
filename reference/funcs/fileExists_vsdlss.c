/* VSDLSS function (decompiled by Ghidra) */
/* name: fileExists_vsdlss  addr: 009f3ac0  size: 5d */
#include "vsdlss_ref.h"

/* signature: bool fileExists_vsdlss(int param_1); */

bool fileExists_vsdlss(int param_1)

{
  FILE *__stream;
  char local_18 [24];
  
  if (*(int *)(&DAT_00e3c870 + (long)param_1 * 0x20) == 0) {
    local_18[0] = 'r';
    local_18[1] = '\0';
  }
  else if (*(int *)(&DAT_00e3c870 + (long)param_1 * 0x20) == 1) {
    local_18[2] = 0;
    local_18[0] = 'r';
    local_18[1] = 'b';
  }
  __stream = fopen(*(char **)(&DAT_00e3c860 + (long)param_1 * 0x20),local_18);
  if (__stream != (FILE *)0x0) {
    fclose(__stream);
  }
  return __stream != (FILE *)0x0;
}


