/* driver: LMBKdFIBkyFKurXSPAIPP addr=0095ab20 size=31 */
#include "vsdlss_ref.h"


void LMBKdFIBkyFKurXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"w");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x171,
                    "File %s cannot be opened in write text mode.\nPlease check permission\n",
                    param_1);
}


