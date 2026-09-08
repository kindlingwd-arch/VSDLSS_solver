/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileW_vsdlss  addr: 009f3680  size: b5 */
#include "vsdlss_ref.h"

/* signature: FILE * openFileW_vsdlss(int param_1); */

FILE * openFileW_vsdlss(int param_1)

{
  char *__filename;
  FILE *pFVar1;
  long lVar2;
  char local_28 [16];
  
  lVar2 = (long)param_1;
  if (*(FILE **)(&DAT_00e3c878 + lVar2 * 0x20) != (FILE *)0x0) {
    fclose(*(FILE **)(&DAT_00e3c878 + lVar2 * 0x20));
    *(undefined8 *)(&DAT_00e3c878 + lVar2 * 0x20) = 0;
  }
  __filename = *(char **)(&DAT_00e3c860 + lVar2 * 0x20);
  if (*(int *)(&DAT_00e3c870 + lVar2 * 0x20) == 0) {
    local_28[0] = 'w';
    local_28[1] = '\0';
  }
  else if (*(int *)(&DAT_00e3c870 + lVar2 * 0x20) == 1) {
    local_28[2] = 0;
    local_28[0] = 'w';
    local_28[1] = 'b';
  }
  pFVar1 = fopen(__filename,local_28);
  if (pFVar1 == (FILE *)0x0) {
    nrerror_vsdlss(0xe,"fmanager.c",0xd3,__filename,"write");
  }
  *(FILE **)(&DAT_00e3c878 + lVar2 * 0x20) = pFVar1;
  return pFVar1;
}


