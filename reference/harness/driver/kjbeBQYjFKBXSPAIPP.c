/* driver: kjbeBQYjFKBXSPAIPP addr=00995f60 size=53 */
#include "vsdlss_ref.h"


bool kjbeBQYjFKBXSPAIPP(char *param_1,int param_2,FILE *param_3)

{
  char *pcVar1;
  char *pcVar2;
  
  pcVar1 = fgets(param_1,param_2,param_3);
  if (pcVar1 != (char *)0x0) {
    pcVar2 = strchr(param_1,10);
    if (pcVar2 == (char *)0x0) {
      KOBOOLOXSPAIPP(0x31,"mldorder.c",0x45,0x3ff);
    }
    else {
      *pcVar2 = '\0';
    }
  }
  return pcVar1 == (char *)0x0;
}


