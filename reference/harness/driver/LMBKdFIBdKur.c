/* driver: LMBKdFIBdKur addr=009bcad0 size=4e */
#include "vsdlss_ref.h"


FILE * LMBKdFIBdKur(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"w");
  if (pFVar1 != (FILE *)0x0) {
    return pFVar1;
  }
  KOBOOLOXRTOyMMBO(0xe,"utils.c",0x8a1,param_1,"write");
  return (FILE *)0x0;
}


