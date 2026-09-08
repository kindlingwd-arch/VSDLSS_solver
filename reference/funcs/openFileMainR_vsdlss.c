/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileMainR_vsdlss  addr: 009d2430  size: 31 */
#include "vsdlss_ref.h"

/* signature: void openFileMainR_vsdlss(char *param_1); */

void openFileMainR_vsdlss(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"rb");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x1ad,
                    "File %s cannot be opened in read binary mode.\nPlease check existence/permission.\n"
                    ,param_1);
}


