/* driver: DBQYjFKBXSPAIPP addr=0095a990 size=57 */
#include "vsdlss_ref.h"


bool DBQYjFKBXSPAIPP(char *param_1,int param_2,FILE *param_3)

{
  char *pcVar1;
  char *pcVar2;
  
  pcVar1 = fgets(param_1,param_2,param_3);
  if (pcVar1 != (char *)0x0) {
    pcVar2 = strchr(param_1,10);
    if (pcVar2 == (char *)0x0) {
                    /* WARNING: Subroutine does not return */
      CyQyIcOOLOXSPAIPP(9999,"main.c",0x127,"getALine:Some file has lines longer than %ld chars\n",
                        0x3ff);
    }
    *pcVar2 = '\0';
  }
  return pcVar1 == (char *)0x0;
}


