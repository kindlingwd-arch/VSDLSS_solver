/* VSDLSS function (decompiled by Ghidra) */
/* name: getALine_vsdlss  addr: 009d21b0  size: 56 */
#include "vsdlss_ref.h"

/* signature: bool getALine_vsdlss(char *param_1,int param_2,FILE *param_3); */

bool getALine_vsdlss(char *param_1,int param_2,FILE *param_3)

{
  char *pcVar1;
  char *pcVar2;
  
  pcVar1 = fgets(param_1,param_2,param_3);
  if (pcVar1 != (char *)0x0) {
    pcVar2 = strchr(param_1,10);
    if (pcVar2 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
      fatalError_vsdlss(9999,"main.c",0x127,"getALine:Some file has lines longer than %d chars\n",
                        0x3ff);
    }
    *pcVar2 = '\0';
  }
  return pcVar1 == (char *)0x0;
}


