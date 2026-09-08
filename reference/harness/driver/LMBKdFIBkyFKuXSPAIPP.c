/* driver: LMBKdFIBkyFKuXSPAIPP addr=0095ab60 size=31 */
#include "vsdlss_ref.h"


void LMBKdFIBkyFKuXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"wb");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x180,
                    "File %s cannot be opened in write binary mode.\nPlease check permission\n",
                    param_1);
}


