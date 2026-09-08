/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileMainRT_vsdlss  addr: 009d23c0  size: 31 */
#include "vsdlss_ref.h"

/* signature: void openFileMainRT_vsdlss(char *param_1); */

void openFileMainRT_vsdlss(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"r");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",399,
                    "File %s cannot be opened in read text mode.\nPlease check existence/permission.\n"
                    ,param_1);
}


