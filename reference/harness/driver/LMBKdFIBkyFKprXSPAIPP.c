/* driver: LMBKdFIBkyFKprXSPAIPP addr=0095aba0 size=31 */
#include "vsdlss_ref.h"


void LMBKdFIBkyFKprXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"r");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",399,
                    "File %s cannot be opened in read text mode.\nPlease check existence/permission.\n"
                    ,param_1);
}


