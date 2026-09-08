/* driver: LMBKdFIBkyFKpXSPAIPP addr=0095ac10 size=31 */
#include "vsdlss_ref.h"


void LMBKdFIBkyFKpXSPAIPP(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"rb");
  if (pFVar1 != (FILE *)0x0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x1ad,
                    "File %s cannot be opened in read binary mode.\nPlease check existence/permission.\n"
                    ,param_1);
}


