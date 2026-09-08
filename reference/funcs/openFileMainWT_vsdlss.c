/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileMainWT_vsdlss  addr: 009d2340  size: 31 */
#include "vsdlss_ref.h"

/* signature: void openFileMainWT_vsdlss(char *param_1); */

void openFileMainWT_vsdlss(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"w");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x171,
                    "File %s cannot be opened in write text mode.\nPlease check permission\n",
                    param_1);
}


