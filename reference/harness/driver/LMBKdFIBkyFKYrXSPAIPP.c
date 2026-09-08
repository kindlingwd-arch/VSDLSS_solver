/* driver: LMBKdFIBkyFKYrXSPAIPP addr=0095aae0 size=31 */
#include "vsdlss_ref.h"


void LMBKdFIBkyFKYrXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"a");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x162,
                    "File %s cannot be opened in append text mode.\nPlease check permission\n",
                    param_1);
}


