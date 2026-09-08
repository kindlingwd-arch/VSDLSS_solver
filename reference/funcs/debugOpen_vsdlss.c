/* VSDLSS function (decompiled by Ghidra) */
/* name: debugOpen_vsdlss  addr: 009f8ff0  size: a8 */
#include "vsdlss_ref.h"

/* signature: FILE * debugOpen_vsdlss(char *param_1); */

FILE * debugOpen_vsdlss(char *param_1)

{
  FILE *pFVar1;
  
  if (DAT_00e3d064 == '\0') {
    DAT_00e3d064 = 0;
  }
  else {
    DAT_00e3d064 = 0;
    pFVar1 = fopen("debugprint.cas","r");
    if (pFVar1 == (FILE *)0x0) {
      DAT_00ec4148 = 0;
      return (FILE *)0x0;
    }
    DAT_00ec4148 = '\x01';
    fclose(pFVar1);
  }
  if (DAT_00ec4148 != '\0') {
    pFVar1 = fopen(param_1,"w");
    if (pFVar1 == (FILE *)0x0) {
      nrerror_vsdlss(0xe,"logfileutils.c",0x136,param_1,&DAT_00b20e86);
    }
    return pFVar1;
  }
  return (FILE *)0x0;
}


