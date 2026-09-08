/* VSDLSS function (decompiled by Ghidra) */
/* name: openFileMainW_vsdlss  addr: 009d2380  size: 31 */
#include "vsdlss_ref.h"

/* signature: void openFileMainW_vsdlss(char *param_1); */

void openFileMainW_vsdlss(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"wb");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  fatalError_vsdlss(9999,"main.c",0x180,
                    "File %s cannot be opened in write binary mode.\nPlease check permission\n",
                    param_1);
}


