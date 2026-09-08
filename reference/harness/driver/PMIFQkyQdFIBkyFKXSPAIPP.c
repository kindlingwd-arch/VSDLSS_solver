/* driver: PMIFQkyQdFIBkyFKXSPAIPP addr=0095ad90 size=1b6 */
#include "vsdlss_ref.h"


void PMIFQkyQdFIBkyFKXSPAIPP
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 param_5)

{
  long lVar1;
  FILE *__stream;
  size_t sVar2;
  void *pvVar3;
  long local_48;
  long local_40 [2];
  
  __stream = (FILE *)LMBKdFIBkyFKpXSPAIPP();
  sVar2 = fread(&local_48,8,1,__stream);
  if (sVar2 != 1) {
                    /* WARNING: Subroutine does not return */
    CyQyIcOOLOXSPAIPP(9999,"main.c",0x202,"splitMatFileMain failed: Reading %ld but read %ld\n",1,
                      sVar2);
  }
  sVar2 = fread(local_40,8,1,__stream);
  lVar1 = local_48;
  if (sVar2 == 1) {
    pvVar3 = tc_malloc(local_48 * 8);
    OByAbtB_QLOdOLJdMkyFKXSPAIPP(__stream,lVar1,pvVar3);
    TOFQBbtB_QLOrLdKkyFKXSPAIPP(param_2,local_48,pvVar3);
    tc_cfree(pvVar3);
    lVar1 = local_40[0];
    pvVar3 = tc_malloc(local_40[0] * 8);
    OByAbtB_QLOdOLJdMkyFKXSPAIPP(__stream,lVar1,pvVar3);
    TOFQBbtB_QLOrLdKkyFKXSPAIPP(param_5,local_40[0],pvVar3);
    tc_cfree(pvVar3);
    lVar1 = local_40[0];
    pvVar3 = tc_malloc(local_40[0] * 8);
    OByAgtB_QLOdOLJdMkyFKXSPAIPP(__stream,lVar1,pvVar3);
    TOFQBgtB_QLOrLdKkyFKXSPAIPP(param_3,local_40[0],pvVar3);
    tc_cfree(pvVar3);
    lVar1 = local_40[0];
    pvVar3 = tc_malloc(local_40[0] * 8);
    OByAgtB_QLOdOLJdMkyFKXSPAIPP(__stream,lVar1,pvVar3);
    TOFQBgtB_QLOrLdKkyFKXSPAIPP(param_4,local_40[0],pvVar3);
    tc_cfree(pvVar3);
    fclose(__stream);
    return;
  }
                    /* WARNING: Subroutine does not return */
  CyQyIcOOLOXSPAIPP(9999,"main.c",0x20a,"splitMatFileMain failed: Reading %ld but read %ld\n",1,
                    sVar2);
}


