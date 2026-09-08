/* driver: TOFQBbtB_QLOrLdKkyFKXSPAIPP addr=0095ac50 size=57 */
#include "vsdlss_ref.h"


void TOFQBbtB_QLOrLdKkyFKXSPAIPP(undefined8 param_1,size_t param_2,void *param_3)

{
  FILE *__s;
  size_t sVar1;
  
  __s = (FILE *)LMBKdFIBkyFKuXSPAIPP();
  sVar1 = fwrite(param_3,8,param_2,__s);
  if (sVar1 == param_2) {
    fclose(__s);
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x1bd,"writeDVectorToFnMain failed: Writing %ld but written %ld\n"
                    ,param_2,sVar1);
}


