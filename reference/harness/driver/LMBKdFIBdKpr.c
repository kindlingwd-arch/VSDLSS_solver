/* driver: LMBKdFIBdKpr addr=009bca70 size=4e */
#include "vsdlss_ref.h"


FILE * LMBKdFIBdKpr(char *param_1)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(param_1,"r");
  if (pFVar1 != (FILE *)0x0) {
    return pFVar1;
  }
  KOBOOLOXRTOyMMBO(0xe,"utils.c",0x893,param_1,"read");
  return (FILE *)0x0;
}


