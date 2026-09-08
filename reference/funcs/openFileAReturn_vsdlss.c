/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileAReturn_vsdlss  addr: 009f38f0  size: 57 */
#include "vsdlss_ref.h"

/* signature: void openFileAReturn_vsdlss(int param_1); */

void openFileAReturn_vsdlss(int param_1)

{
  long lVar1;
  FILE *pFVar2;
  char local_18 [16];
  
  lVar1 = (long)param_1 * 0x20;
  if (*(int *)(&DAT_00e3c870 + lVar1) == 0) {
    local_18[0] = 'a';
    local_18[1] = '\0';
  }
  else if (*(int *)(&DAT_00e3c870 + lVar1) == 1) {
    local_18[2] = 0;
    local_18[0] = 'a';
    local_18[1] = 'b';
  }
  pFVar2 = fopen(*(char **)(&DAT_00e3c860 + lVar1),local_18);
  *(FILE **)(&DAT_00e3c878 + (long)param_1 * 0x20) = pFVar2;
  return;
}


