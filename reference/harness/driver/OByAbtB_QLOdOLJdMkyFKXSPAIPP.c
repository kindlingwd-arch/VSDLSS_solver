/* driver: OByAbtB_QLOdOLJdMkyFKXSPAIPP addr=0095ad10 size=3f */
#include "vsdlss_ref.h"


void OByAbtB_QLOdOLJdMkyFKXSPAIPP(FILE *param_1,size_t param_2,void *param_3)

{
  size_t sVar1;
  
  sVar1 = fread(param_3,8,param_2,param_1);
  if (sVar1 == param_2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x1df,"readDVectorFromFpMain failed: Reading %ld but read %ld\n",
                    param_2,sVar1);
}


